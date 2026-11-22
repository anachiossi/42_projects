"""Tests for MazeConfig.

Run from the repository root::

    python3 -m unittest discover -s tests -t .
"""

import os
import tempfile
import unittest

from app.config import MazeConfigError, read_config
from mazegen import MazeConfig

VALID = """\
# the default configuration, with a comment and a blank line

WIDTH=20
HEIGHT=15
ENTRY=0,0
EXIT=19,14
OUTPUT_FILE=maze.txt
PERFECT=True
"""


class MazeConfigTest(unittest.TestCase):
    """Common helpers for the cases below."""

    def write(self, text: str) -> str:
        """Write *text* to a temporary file and return its path."""
        handle = tempfile.NamedTemporaryFile(
            "w", suffix=".txt", delete=False, encoding="utf-8"
        )
        with handle as stream:
            stream.write(text)
        self.addCleanup(os.unlink, handle.name)
        return handle.name

    def load(self, text: str) -> MazeConfig:
        """Read *text* as a configuration file, keeping the settings."""
        return read_config(self.write(text))[0]

    def output_of(self, text: str) -> str:
        """Read *text* as a configuration file, keeping OUTPUT_FILE."""
        return read_config(self.write(text))[1]

    def assert_rejected(self, text: str) -> str:
        """Assert *text* is refused, and return the message."""
        with self.assertRaises(MazeConfigError) as caught:
            self.load(text)
        return str(caught.exception)


class TestValidConfig(MazeConfigTest):
    """A well formed file is read correctly."""

    def test_reads_every_value(self) -> None:
        config = self.load(VALID)
        self.assertEqual(config.width, 20)
        self.assertEqual(config.height, 15)
        self.assertEqual(config.entry_cell, (0, 0))
        self.assertEqual(self.output_of(VALID), "maze.txt")
        self.assertIs(config.perfect, True)

    def test_exit_keeps_the_column_first(self) -> None:
        """19,14 stays (19, 14): 19 is the column, 14 the row."""
        self.assertEqual(self.load(VALID).exit_cell, (19, 14))

    def test_numbers_are_not_strings(self) -> None:
        config = self.load(VALID)
        self.assertIsInstance(config.width, int)
        self.assertIsInstance(config.perfect, bool)

    def test_keys_are_case_insensitive(self) -> None:
        self.assertEqual(self.load(VALID.lower()).width, 20)

    def test_extra_keys_are_allowed(self) -> None:
        """The subject permits additional keys such as SEED."""
        self.assertEqual(self.load(VALID + "SEED=42\n").width, 20)

    def test_value_may_contain_an_equals_sign(self) -> None:
        text = VALID.replace("maze.txt", "out=put.txt")
        self.assertEqual(self.output_of(text), "out=put.txt")

    def test_perfect_accepts_false(self) -> None:
        text = VALID.replace("PERFECT=True", "PERFECT=False")
        self.assertIs(self.load(text).perfect, False)


class TestOptionalKeys(MazeConfigTest):
    """SEED and the two ALGORITHM keys may be absent, present, wrong."""

    def test_absent_optional_keys_are_none(self) -> None:
        config = self.load(VALID)
        self.assertIsNone(config.seed)
        self.assertIsNone(config.gen_algorithm)
        self.assertIsNone(config.solve_algorithm)

    def test_seed_is_read_as_an_int(self) -> None:
        config = self.load(VALID + "SEED=42\n")
        self.assertEqual(config.seed, 42)
        self.assertIsInstance(config.seed, int)

    def test_algorithm_is_read_as_a_string(self) -> None:
        config = self.load(VALID + "GEN_ALGORITHM=backtracker\n")
        self.assertEqual(config.gen_algorithm, "backtracker")

    def test_the_two_algorithms_are_read_apart(self) -> None:
        """One key names who carves, the other who walks it."""
        config = self.load(VALID + "GEN_ALGORITHM=kruskal\n"
                                   "SOLVE_ALGORITHM=bfs\n")
        self.assertEqual(config.gen_algorithm, "kruskal")
        self.assertEqual(config.solve_algorithm, "bfs")

    def test_both_optional_keys_together(self) -> None:
        config = self.load(VALID + "SEED=7\nGEN_ALGORITHM=kruskal\n")
        self.assertEqual((config.seed, config.gen_algorithm), (7, "kruskal"))

    def test_optional_keys_are_case_insensitive(self) -> None:
        self.assertEqual(self.load(VALID + "seed=99\n").seed, 99)

    def test_seed_must_be_a_number(self) -> None:
        message = self.assert_rejected(VALID + "SEED=abc\n")
        self.assertIn("SEED", message)

    def test_a_still_unknown_key_is_ignored(self) -> None:
        """BANANA is neither mandatory nor optional: simply ignored."""
        self.assertEqual(self.load(VALID + "BANANA=yes\n").width, 20)


class TestMalformedFile(MazeConfigTest):
    """A file that cannot be read at all."""

    def test_missing_file(self) -> None:
        with self.assertRaises(MazeConfigError):
            read_config("no_such_file_here.txt")

    def test_line_without_equals(self) -> None:
        message = self.assert_rejected(VALID + "GARBAGE\n")
        self.assertIn("KEY=VALUE", message)

    def test_missing_mandatory_key(self) -> None:
        message = self.assert_rejected(VALID.replace("HEIGHT=15\n", ""))
        self.assertIn("HEIGHT", message)


class TestBadValues(MazeConfigTest):
    """Keys are all there, but a value cannot be used."""

    def test_width_is_not_a_number(self) -> None:
        message = self.assert_rejected(VALID.replace("WIDTH=20", "WIDTH=xx"))
        self.assertIn("WIDTH", message)

    def test_unknown_boolean_word(self) -> None:
        """PERFECT=Tru must fail, not quietly become False."""
        text = VALID.replace("PERFECT=True", "PERFECT=Tru")
        self.assertIn("PERFECT", self.assert_rejected(text))

    def test_coordinate_needs_exactly_two_numbers(self) -> None:
        text = VALID.replace("ENTRY=0,0", "ENTRY=1,2,3")
        self.assertIn("ENTRY", self.assert_rejected(text))

    def test_coordinate_must_be_numeric(self) -> None:
        text = VALID.replace("ENTRY=0,0", "ENTRY=a,b")
        self.assertIn("ENTRY", self.assert_rejected(text))


class TestSizeLimits(MazeConfigTest):
    """WIDTH and HEIGHT are bounded by MIN_SIZE and MAX_SIZE."""

    def test_width_below_minimum(self) -> None:
        self.assert_rejected(VALID.replace("WIDTH=20", "WIDTH=0"))

    def test_negative_width(self) -> None:
        self.assert_rejected(VALID.replace("WIDTH=20", "WIDTH=-5"))

    def test_height_above_maximum(self) -> None:
        self.assert_rejected(VALID.replace("HEIGHT=15", "HEIGHT=99999"))

    def test_smallest_accepted_maze(self) -> None:
        text = ("WIDTH=11\nHEIGHT=9\nENTRY=0,0\nEXIT=10,8\n"
                "OUTPUT_FILE=maze.txt\nPERFECT=True\n")
        config = self.load(text)
        self.assertEqual((config.width, config.height), (11, 9))

    def test_too_small_for_the_42(self) -> None:
        text = ("WIDTH=2\nHEIGHT=1\nENTRY=0,0\nEXIT=1,0\n"
                "OUTPUT_FILE=maze.txt\nPERFECT=True\n")
        self.assertIn("42", self.assert_rejected(text))

    def test_one_column_short_for_the_42(self) -> None:
        text = ("WIDTH=10\nHEIGHT=9\nENTRY=0,0\nEXIT=9,8\n"
                "OUTPUT_FILE=maze.txt\nPERFECT=True\n")
        self.assertIn("42", self.assert_rejected(text))

    def test_one_row_short_for_the_42(self) -> None:
        text = ("WIDTH=11\nHEIGHT=8\nENTRY=0,0\nEXIT=10,7\n"
                "OUTPUT_FILE=maze.txt\nPERFECT=True\n")
        self.assertIn("42", self.assert_rejected(text))


class TestEntryAndExit(MazeConfigTest):
    """The two cells must exist and be different."""

    def test_exit_outside_the_maze(self) -> None:
        text = VALID.replace("EXIT=19,14", "EXIT=19,99")
        self.assertIn("EXIT", self.assert_rejected(text))

    def test_entry_negative(self) -> None:
        text = VALID.replace("ENTRY=0,0", "ENTRY=-1,0")
        self.assertIn("ENTRY", self.assert_rejected(text))

    def test_entry_equal_to_exit(self) -> None:
        text = VALID.replace("EXIT=19,14", "EXIT=0,0")
        self.assertIn("different", self.assert_rejected(text))

    def test_column_and_row_are_not_swapped(self) -> None:
        """EXIT=14,19 is out of bounds: 19 rows do not exist."""
        text = VALID.replace("EXIT=19,14", "EXIT=14,19")
        self.assertIn("EXIT", self.assert_rejected(text))


class TestSettingsObject(MazeConfigTest):
    """What the file is read into is mazegen's settings object itself."""

    def test_it_maps_every_field(self) -> None:
        config = self.load(VALID + "SEED=7\nGEN_ALGORITHM=kruskal\n"
                                   "SOLVE_ALGORITHM=bfs\n")
        self.assertIsInstance(config, MazeConfig)
        self.assertEqual(config.width, 20)
        self.assertEqual(config.height, 15)
        self.assertEqual(config.entry_cell, (0, 0))
        self.assertEqual(config.exit_cell, (19, 14))
        self.assertTrue(config.perfect)
        self.assertEqual(config.seed, 7)
        self.assertEqual(config.gen_algorithm, "kruskal")
        self.assertEqual(config.solve_algorithm, "bfs")

    def test_absent_optional_keys_stay_none(self) -> None:
        config = self.load(VALID)
        self.assertIsNone(config.seed)
        self.assertIsNone(config.gen_algorithm)
        self.assertIsNone(config.solve_algorithm)

    def test_output_file_is_not_in_the_settings(self) -> None:
        """OUTPUT_FILE comes back beside the settings, never inside."""
        config, output_file = read_config(self.write(VALID))
        self.assertFalse(hasattr(config, "output_file"))
        self.assertEqual(output_file, "maze.txt")


if __name__ == "__main__":
    unittest.main()
