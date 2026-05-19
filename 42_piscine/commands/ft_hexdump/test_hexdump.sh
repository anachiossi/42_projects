#!/bin/bash

# ============================================
# Mini Moulinette — ft_hexdump tester
# Usage: bash test_hexdump.sh
# Run from your ex03/ directory
# ============================================

BINARY="./ft_hexdump"
PASS=0
FAIL=0
GREEN='\033[0;32m'
RED='\033[0;31m'
CYAN='\033[0;36m'
NC='\033[0m'

pass() { echo -e "${GREEN}[OK]${NC} $1"; ((PASS++)); }
fail() { echo -e "${RED}[KO]${NC} $1"; ((FAIL++)); }
header() { echo -e "\n${CYAN}--- $1 ---${NC}"; }

# Check binary exists
if [ ! -f "$BINARY" ]; then
    echo -e "${RED}ERROR: $BINARY not found. Run 'make' first.${NC}"
    exit 1
fi

# ============================================
# Create test files
# ============================================

# 1. Empty file
> /tmp/empty.bin

# 2. One byte
printf '\x41' > /tmp/one_byte.bin

# 3. Exactly 16 bytes
printf 'ABCDEFGHIJKLMNOP' > /tmp/sixteen.bin

# 4. Exactly 17 bytes (one full line + one partial)
printf 'ABCDEFGHIJKLMNOPQ' > /tmp/seventeen.bin

# 5. File with null bytes
printf 'Hello\x00World\x00\x00\x00\x00\x00' > /tmp/nulls.bin

# 6. File with repeated lines (triggers *)
python3 -c "import sys; sys.stdout.buffer.write(b'\x20' * 64)" > /tmp/repeated.bin

# 7. Binary file with non-printable bytes
python3 -c "import sys; sys.stdout.buffer.write(bytes(range(256)))" > /tmp/binary.bin

# 8. Exactly 32 bytes, second 16 different from first
printf 'AAAAAAAAAAAAAAAA' > /tmp/two_diff.bin
printf 'BBBBBBBBBBBBBBBB' >> /tmp/two_diff.bin

# ============================================
# Run tests
# ============================================

run_test() {
    local name="$1"
    local file="$2"
    local use_flag="$3"   # "flag" or "noflag"
    local expect_err="$4" # "err" or ""

    if [ "$use_flag" = "flag" ]; then
        actual_out=$($BINARY -C "$file" 2>/tmp/ft_err.txt)
        expected_out=$(hexdump -C "$file" 2>/dev/null)
    else
        actual_out=$($BINARY "$file" 2>/tmp/ft_err.txt)
        expected_out=$(hexdump -C "$file" 2>/dev/null)
    fi

    if [ "$expect_err" = "err" ]; then
        if [ -s /tmp/ft_err.txt ]; then
            pass "$name"
        else
            fail "$name (no error message on stderr)"
        fi
    else
        if [ "$actual_out" = "$expected_out" ]; then
            pass "$name"
        else
            fail "$name"
            echo "  EXPECTED:"
            echo "$expected_out" | head -5
            echo "  GOT:"
            echo "$actual_out" | head -5
        fi
    fi
}

run_test_multi() {
    local name="$1"
    local file1="$2"
    local file2="$3"

    actual_out=$($BINARY -C "$file1" "$file2" 2>/tmp/ft_err.txt)
    expected_out=$(hexdump -C "$file1"; hexdump -C "$file2")

    if [ "$actual_out" = "$expected_out" ]; then
        pass "$name"
    else
        fail "$name"
        echo "  EXPECTED:"
        echo "$expected_out" | head -8
        echo "  GOT:"
        echo "$actual_out" | head -8
    fi
}

run_test_noargs() {
    actual_out=$(echo "hello" | $BINARY 2>/tmp/ft_err.txt)
    expected_out=$(echo "hello" | hexdump -C)
    if [ "$actual_out" = "$expected_out" ]; then
        pass "No arguments (stdin)"
    else
        fail "No arguments (stdin)"
        echo "  EXPECTED:"
        echo "$expected_out"
        echo "  GOT:"
        echo "$actual_out"
    fi
}

# ============================================
# TEST CASES
# ============================================

header "Basic functionality"
run_test "Empty file"              "/tmp/empty.bin"     "flag"
run_test "One byte"                "/tmp/one_byte.bin"  "flag"
run_test "Exactly 16 bytes"        "/tmp/sixteen.bin"   "flag"
run_test "Exactly 17 bytes"        "/tmp/seventeen.bin" "flag"

header "Special content"
run_test "File with null bytes"    "/tmp/nulls.bin"     "flag"
run_test "Binary file (0-255)"     "/tmp/binary.bin"    "flag"
run_test "Two different lines"     "/tmp/two_diff.bin"  "flag"

header "Repeated lines (star)"
run_test "Repeated lines *"        "/tmp/repeated.bin"  "flag"

header "Flag handling"
run_test "Without -C flag"         "/tmp/sixteen.bin"   "noflag"
run_test_multi "Multiple files"    "/tmp/one_byte.bin"  "/tmp/sixteen.bin"

header "Error handling"
run_test_noargs
run_test "Bad filename"            "/tmp/doesnotexist_42.bin" "flag" "err"

# ============================================
# SUMMARY
# ============================================

echo ""
echo "==============================="
echo -e "  ${GREEN}PASS: $PASS${NC}  |  ${RED}FAIL: $FAIL${NC}"
echo "==============================="

# Cleanup
rm -f /tmp/empty.bin /tmp/one_byte.bin /tmp/sixteen.bin /tmp/seventeen.bin
rm -f /tmp/nulls.bin /tmp/repeated.bin /tmp/binary.bin /tmp/two_diff.bin
rm -f /tmp/ft_err.txt
