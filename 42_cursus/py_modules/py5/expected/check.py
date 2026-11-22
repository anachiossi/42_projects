#!/usr/bin/env python3
"""check.py - behavioural tester for py05 Data Pipeline.

    ./check.py                run every section
    ./check.py -v             show every case, pass or fail
    ./check.py numeric        run only sections whose name contains "numeric"
    ./check.py -v log

Every run also writes expected/traces.txt: one line per case with what was
sent, what came back, and what was expected. Override with
TRACE_FILE=/somewhere/else.txt ./check.py

WHY THIS IS PYTHON AND NOT BASH, unlike py04's check.sh:
py04 was a CLI - argv in, stdout/stderr out - so diffing golden files was
exactly the right shape. ex0 has no CLI surface at all: one fixed program
output, and three classes whose interesting behaviour is invisible from the
outside. A golden diff would only prove main() still prints what it printed
yesterday. Every bug found while writing this exercise was logic that never
reached stdout:

  - validate() returning False for a valid list, because the success path
    fell through to the single-item return
  - an empty-list guard that computed its answer and threw it away
  - a class attribute named `output` shadowing the inherited output() method

So the cases below poke the objects directly. The last section still checks
the program's real stdout, but only for the lines the subject pins down -
main() is yours to arrange, and an exact diff would fight you every time you
add a test line to it.

Nothing here is imported by your submission. Standard library only.
"""

import contextlib
import datetime
import importlib.util
import io
import os
import pathlib
import sys
import typing

ROOT = pathlib.Path(__file__).resolve().parent.parent
# Path("") is PosixPath(".") - truthy, and a directory. Test the string.
_TRACE_ENV = os.environ.get("TRACE_FILE", "")
TRACE = pathlib.Path(_TRACE_ENV) if _TRACE_ENV else None

RED = "\033[31m"
GRN = "\033[32m"
YEL = "\033[33m"
DIM = "\033[2m"
OFF = "\033[0m"
if not sys.stdout.isatty():
    RED = GRN = YEL = DIM = OFF = ""


class Report:
    """Counts, prints and traces every case."""

    def __init__(self, verbose: bool, filt: str, trace: pathlib.Path) -> None:
        self.verbose = verbose
        self.filt = filt.lower()
        self.trace = trace
        self.passed = 0
        self.failed = 0
        self.skipped = 0
        self.section = ""
        self.active = True
        stamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self.trace.write_text(f"py05 trace - {stamp}\n")
        self._trace(f"filter: {self.filt or '<none>'}\n")

    def _trace(self, line: str) -> None:
        with self.trace.open("a") as handle:
            handle.write(line)

    def start(self, section: str) -> None:
        """Begin a section. Honours the name filter."""
        self.section = section
        self.active = self.filt in section.lower()
        if self.active:
            print(f"\n  {section}")
            self._trace(f"\n=== {section} ===\n")

    def check(self, label: str, got: typing.Any, want: typing.Any) -> None:
        if not self.active:
            return
        ok = got == want and type(got) is type(want)
        self._trace(
            f"    {'ok ' if ok else 'FAIL'}  {label}\n"
            f"          got  {got!r}\n"
            f"          want {want!r}\n"
        )
        if ok:
            self.passed += 1
            if self.verbose:
                print(f"    {GRN}ok{OFF}      {label}")
        else:
            self.failed += 1
            print(f"    {RED}FAIL{OFF}    {label}")
            print(f"            {YEL}got {OFF} {got!r}")
            print(f"            {YEL}want{OFF} {want!r}")

    def crash(self, exc: BaseException) -> None:
        """A section blew up. Count it, report it, keep going.

        Without this the first AttributeError takes the whole run down and
        you see a traceback instead of results - which is exactly what
        happened when a class attribute shadowed output(). A crash is a
        result, not a reason to abandon the remaining sections.
        """
        self.failed += 1
        name = type(exc).__name__
        print(f"    {RED}CRASH{OFF}   {self.section}")
        print(f"            {YEL}{name}{OFF}: {exc}")
        self._trace(f"    CRASH  {self.section}: {name}: {exc}\n")

    def skip(self, label: str, why: str) -> None:
        self.skipped += 1
        print(f"    {YEL}SKIP{OFF}    {label} {DIM}({why}){OFF}")
        self._trace(f"    SKIP  {label} ({why})\n")

    def total(self) -> int:
        print(
            f"\n  {self.passed} passed, {self.failed} failed, "
            f"{self.skipped} skipped"
        )
        self._trace(
            f"\n{self.passed} passed, {self.failed} failed, "
            f"{self.skipped} skipped\n"
        )
        print(f"  {DIM}trace: {self.trace}{OFF}")
        return 1 if self.failed else 0


def written(path: pathlib.Path) -> bool:
    """Exists AND has content.

    The scaffold creates every exN file empty, so is_file() alone would call
    an untouched ex1 "present" and then import a module with no classes in it.
    This is py04's `[ -s "$src" ]` test.
    """
    return path.is_file() and path.stat().st_size > 0


def load(exdir: str, filename: str) -> typing.Any:
    """Import exN/filename.py by path.

    The module's `if __name__ == "__main__"` guard keeps main() from firing
    here, because __name__ becomes the file's stem rather than "__main__".
    That guard is the only reason this tester can touch your classes without
    the whole demo printing first.
    """
    path = ROOT / exdir / filename
    if not written(path):
        return None
    spec = importlib.util.spec_from_file_location(path.stem, path)
    if spec is None or spec.loader is None:
        return None
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def capture(fn: typing.Callable[[], typing.Any]) -> list[str]:
    """Run fn() and return whatever it printed, as a list of lines."""
    buffer = io.StringIO()
    with contextlib.redirect_stdout(buffer):
        fn()
    return buffer.getvalue().splitlines()


def raises(fn: typing.Callable[[], typing.Any]) -> bool:
    """True if calling fn() raises anything."""
    try:
        fn()
    except Exception:
        return True
    return False


# --- validate matrices ------------------------------------------------------
# (value, expected). Grouped by meaning, not by expected answer, so a gap in
# the coverage is visible as a missing GROUP rather than a missing line.

NUMERIC_CASES: list[tuple[typing.Any, bool]] = [
    # plain numbers
    (42, True), (-1, True), (0, True), (3.14, True), (-2.5, True),
    (0xFF, True),                  # hex literal is just an int by now
    (1e308, True),
    # bool is a subclass of int - must NOT count as numeric
    (True, False), (False, False),
    # strings belong to Text, even numeric-looking ones
    ("42", False), ("Hello", False), ("", False),
    # other scalars
    (None, False), (complex(1, 2), False),
    # batches
    ([1, 2, 3], True), ([1, 2.5, -3], True), ([0], True),
    ([], False),                   # empty batch stores nothing - reject
    ([1, "foo"], False),           # all-or-nothing
    ([True], False), ([[1, 2]], False),
    # wrong containers
    ((1, 2), False), ({"a": "b"}, False), ({1, 2}, False),
]

TEXT_CASES: list[tuple[typing.Any, bool]] = [
    # any str, whatever it contains
    ("Hello", True), ("", True), ("42", True), ("a\0b", True),
    # NUL is an ordinary character in Python - not a terminator as in C
    # non-strings
    (42, False), (3.14, False), (True, False), (None, False),
    # batches
    (["a", "b"], True), ([""], True),
    ([], False), (["a", 42], False), ([["a"]], False),
    # wrong containers
    (("a",), False), ({"a": "b"}, False),
]

LOG_CASES: list[tuple[typing.Any, bool]] = [
    # dict of str -> str
    ({"log_level": "INFO", "log_message": "System ready"}, True),
    ({"a": "b"}, True),
    ({}, False),                   # nothing to render
    ({"a": 1}, False),             # value not a str
    ({1: "a"}, False),             # key not a str
    # batches
    ([{"a": "b"}], True),
    ([{"a": "b"}, {"c": "d"}], True),
    ([], False), ([{}], False), ([{"a": "b"}, "x"], False),
    ([[{"a": "b"}]], False),
    # not a dict at all
    ("Hello", False), (42, False), (None, False),
]


def section_validate(r: Report, mod: typing.Any) -> None:
    for name, cls_name, cases in (
        ("Numeric - validate", "NumericProcessor", NUMERIC_CASES),
        ("Text - validate", "TextProcessor", TEXT_CASES),
        ("Log - validate", "LogProcessor", LOG_CASES),
    ):
        r.start(name)
        if not r.active:
            continue
        cls = getattr(mod, cls_name)
        for value, want in cases:
            proc = cls()
            r.check(f"validate({value!r})", proc.validate(value), want)


def section_ingest(r: Report, mod: typing.Any) -> None:
    r.start("Numeric - ingest & store")
    if r.active:
        p = mod.NumericProcessor()
        p.ingest([1, 2.5, -3])
        r.check(
            "batch stored as strings",
            p.processed_data,
            [(0, "1"), (1, "2.5"), (2, "-3")],
        )
        p.ingest(7)
        # ranks count items ever ingested, not items in this batch - ex1's
        # "total N items processed" stat reads exactly this counter
        r.check("rank continues across calls", p.processed_data[-1], (3, "7"))
        q = mod.NumericProcessor()
        q.ingest(0xFF)
        r.check("hex literal stores decimal", q.processed_data, [(0, "255")])

    r.start("Text - ingest & store")
    if r.active:
        p = mod.TextProcessor()
        p.ingest("Hello")
        # the trap: str is iterable, so a looser "is it iterable?" branch
        # would store 5 characters with 5 separate ranks
        r.check("single str is ONE item", p.processed_data, [(0, "Hello")])
        p.ingest(["Nexus", "World"])
        r.check(
            "batch appends in order",
            p.processed_data,
            [(0, "Hello"), (1, "Nexus"), (2, "World")],
        )

    r.start("Log - ingest & store")
    if r.active:
        p = mod.LogProcessor()
        p.ingest({"log_level": "NOTICE",
                  "log_message": "Connection to server"})
        r.check(
            "values joined, keys discarded",
            p.processed_data,
            [(0, "NOTICE: Connection to server")],
        )
        q = mod.LogProcessor()
        q.ingest([
            {"log_level": "NOTICE", "log_message": "Connection to server"},
            {"log_level": "ERROR", "log_message": "Unauthorized access!!"},
        ])
        r.check(
            "batch of dicts",
            q.processed_data,
            [
                (0, "NOTICE: Connection to server"),
                (1, "ERROR: Unauthorized access!!"),
            ],
        )


def section_atomic(r: Report, mod: typing.Any) -> None:
    """A rejected batch must leave NOTHING behind - see the Notion page."""
    r.start("Atomicity - a failed ingest changes nothing")
    if not r.active:
        return
    for cls_name, good, bad in (
        ("NumericProcessor", [10, 20], [1, "foo", 3]),
        ("TextProcessor", ["a", "b"], ["a", 42]),
        ("LogProcessor", [{"k": "v"}], [{"k": "v"}, "x"]),
    ):
        p = getattr(mod, cls_name)()
        p.ingest(good)
        before = list(p.processed_data)
        index_before = p.index
        r.check(f"{cls_name}: bad batch raises", raises(
            lambda: p.ingest(bad)), True)
        r.check(f"{cls_name}: store unchanged", p.processed_data, before)
        r.check(f"{cls_name}: index unchanged", p.index, index_before)


def section_output(r: Report, mod: typing.Any) -> None:
    r.start("Base - output() and ranks")
    if not r.active:
        return
    p = mod.TextProcessor()
    p.ingest(["a", "b", "c"])
    r.check("output() returns oldest first", p.output(), (0, "a"))
    r.check("output() removes it", len(p.processed_data), 2)
    r.check("then the next oldest", p.output(), (1, "b"))
    r.check("index does NOT rewind", p.index, 3)
    p.output()
    r.check("empty store raises", raises(p.output), True)


def section_contract(r: Report, mod: typing.Any) -> None:
    r.start("Base - abstract contract")
    if not r.active:
        return
    r.check(
        "DataProcessor cannot be instantiated",
        raises(mod.DataProcessor),
        True,
    )
    for cls_name in ("NumericProcessor", "TextProcessor", "LogProcessor"):
        p = getattr(mod, cls_name)()
        r.check(f"{cls_name}.name is a str", isinstance(p.name, str), True)
        r.check(f"{cls_name}.label is a str", isinstance(p.label, str), True)
        # regression: a class attribute named `output` would shadow the
        # inherited method and make this call a TypeError
        r.check(
            f"{cls_name}.output is callable", callable(p.output), True)


def section_routing(r: Report, mod: typing.Any) -> None:
    """No value may be claimed by two processors.

    Not required by ex0, but ex1's DataStream routes each element to whichever
    processor validates it. If two ever say yes, the winner depends on
    registration order - a bug that would only surface next exercise.
    """
    r.start("Routing - exactly one processor claims each value")
    if not r.active:
        return
    procs = [
        mod.NumericProcessor(), mod.TextProcessor(), mod.LogProcessor(),
    ]
    values: list[typing.Any] = [
        42, 3.14, "Hello", "42", "",
        {"log_level": "a", "log_message": "b"},
        [1, 2], ["a", "b"], [{"a": "b"}],
    ]
    for value in values:
        takers = [p.name for p in procs if p.validate(value)]
        r.check(f"{value!r} claimed by exactly one", len(takers), 1)

    unclaimed: list[typing.Any] = [None, [], {}, (1, 2), [1, "a"], True]
    for value in unclaimed:
        takers = [p.name for p in procs if p.validate(value)]
        r.check(f"{value!r} claimed by none", len(takers), 0)


def section_stream(r: Report, mod: typing.Any) -> None:
    """DataStream routing, registration and statistics (ex1)."""
    r.start("DataStream - registration and stats")
    if r.active:
        s = mod.DataStream()
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            s.print_processors_stats()
        r.check(
            "empty stream reports no processor",
            out.getvalue().splitlines(),
            ["== DataStream statistics ==", "No processor found, no data"],
        )
        num = mod.NumericProcessor()
        s.register_processor(num)
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            s.print_processors_stats()
        r.check(
            "one processor, nothing ingested",
            out.getvalue().splitlines()[-1],
            "Numeric Processor: total 0 items processed, "
            "remaining 0 on processor",
        )

    r.start("DataStream - routing an unroutable element")
    if r.active:
        s = mod.DataStream()
        s.register_processor(mod.NumericProcessor())
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            s.process_stream(["Hello world", 42, ["Hi", "five"]])
        lines = out.getvalue().splitlines()
        # two elements have no taker, and the stream must NOT abort: the
        # 42 between them still has to be ingested
        r.check("one error line per unroutable element", len(lines), 2)
        r.check(
            "error format",
            lines[0],
            "DataStream error - Can't process element in stream: Hello world",
        )
        r.check("routable element still ingested",
                s.processors[0].processed_data, [(0, "42")])

    r.start("DataStream - total vs remaining")
    if r.active:
        s = mod.DataStream()
        num = mod.NumericProcessor()
        s.register_processor(num)
        s.process_stream([[1, 2, 3, 4]])
        num.output()
        with contextlib.redirect_stdout(io.StringIO()) as out:
            s.print_processors_stats()
        # total counts everything ever ingested and never decreases;
        # remaining is what is still queued
        r.check(
            "consuming lowers remaining, not total",
            out.getvalue().splitlines()[-1],
            "Numeric Processor: total 4 items processed, "
            "remaining 3 on processor",
        )


def section_plugins(r: Report, mod: typing.Any) -> None:
    """Export plugin formats and the duck typing behind them (ex2)."""
    r.start("Plugins - CSV format")
    if r.active:
        csv = mod.CSVPlugin()
        r.check(
            "header, then values comma-joined with no spaces",
            capture(lambda: csv.process_output(
                [(0, "3.14"), (1, "-1"), (2, "2.71")])),
            ["CSV Output:", "3.14,-1,2.71"],
        )
        # ranks are dropped by CSV - only the stored strings appear
        r.check(
            "ranks are not exported",
            capture(lambda: csv.process_output([(7, "a"), (9, "b")])),
            ["CSV Output:", "a,b"],
        )

    r.start("Plugins - JSON format")
    if r.active:
        js = mod.JSONPlugin()
        r.check(
            "item_<rank> keys, quoted values, ', ' between pairs",
            capture(lambda: js.process_output([(3, "42"), (4, "21")])),
            ["JSON Output:", '{"item_3": "42", "item_4": "21"}'],
        )
        r.check(
            "empty batch is still a valid object",
            capture(lambda: js.process_output([])),
            ["JSON Output:", "{}"],
        )

    r.start("Plugins - duck typing, not inheritance")
    if r.active:
        for cls_name in ("CSVPlugin", "JSONPlugin"):
            cls = getattr(mod, cls_name)
            # the subject asks for compatibility "through duck typing":
            # satisfying the protocol by SHAPE, never by subclassing it
            r.check(f"{cls_name} does not subclass ExportPlugin",
                    mod.ExportPlugin in cls.__mro__, False)
            r.check(f"{cls_name}.name is a str",
                    isinstance(cls.name, str), True)
            r.check(f"{cls_name}.process_output is callable",
                    callable(cls.process_output), True)


def section_pipeline(r: Report, mod: typing.Any) -> None:
    """output_pipeline: how much it consumes, and from whom (ex2)."""
    r.start("output_pipeline - nb is a maximum, not a demand")
    if r.active:
        s = mod.DataStream()
        num = mod.NumericProcessor()
        log = mod.LogProcessor()
        s.register_processor(num)
        s.register_processor(log)
        s.process_stream([[1, 2, 3, 4, 5], [{"a": "b"}, {"c": "d"}]])
        out = capture(lambda: s.output_pipeline(3, mod.CSVPlugin()))
        r.check("one block per registered processor",
                out.count("CSV Output:"), 2)
        r.check("numeric exported the 3 asked for", out[1], "1,2,3")
        # log only holds 2 - asking for 3 must export both and stop,
        # not raise IndexError on the third output()
        r.check("log exported only the 2 it had", out[3], "b,d")
        r.check("numeric keeps the remainder", len(num.processed_data), 2)
        r.check("log is drained", len(log.processed_data), 0)

    r.start("output_pipeline - ranks survive across exports")
    if r.active:
        s = mod.DataStream()
        num = mod.NumericProcessor()
        s.register_processor(num)
        s.process_stream([[1, 2, 3, 4, 5]])
        capture(lambda: s.output_pipeline(3, mod.CSVPlugin()))
        # index never rewinds, so the next export starts at item_3 -
        # exactly what the subject's JSON block demonstrates
        r.check(
            "second export continues the numbering",
            capture(lambda: s.output_pipeline(2, mod.JSONPlugin()))[1],
            '{"item_3": "4", "item_4": "5"}',
        )


def section_program(r: Report, mod: typing.Any) -> None:
    """Exact, line-by-line diff against the subject's printed example.

    cases/ex0-main.out is transcribed from the PDF, so this is a genuine
    external reference - not a snapshot of your own output frozen at some
    arbitrary moment. An evaluator with the subject open will compare
    exactly this way, so it is the check that matters most.

    Reported per line rather than as one blob, so a failure names the line
    number and shows both sides - the same idea as py04's pair_lines().
    """
    r.start(f"Program {mod.__name__} - exact match with the subject example")
    if not r.active:
        return
    cases = {"data_processor": "ex0-main.out",
             "data_stream": "ex1-main.out",
             "data_pipeline": "ex2-main.out"}
    case = cases[mod.__name__]
    golden = pathlib.Path(__file__).resolve().parent / "cases" / case
    if not written(golden):
        r.skip("cases/ex0-main.out", "golden file missing")
        return

    buffer = io.StringIO()
    with contextlib.redirect_stdout(buffer):
        mod.main()
    got = buffer.getvalue().splitlines()
    want = golden.read_text().splitlines()

    r.check("line count", len(got), len(want))
    for i in range(max(len(got), len(want))):
        r.check(
            f"line {i + 1}",
            got[i] if i < len(got) else "<missing>",
            want[i] if i < len(want) else "<missing>",
        )


def main() -> int:
    verbose = False
    filt = ""
    for arg in sys.argv[1:]:
        if arg in ("-v", "--verbose"):
            verbose = True
        elif arg.startswith("-"):
            print(f"unknown option: {arg}", file=sys.stderr)
            return 2
        else:
            filt = arg

    trace = TRACE or pathlib.Path(__file__).resolve().parent / "traces.txt"
    r = Report(verbose, filt, trace)

    mod = load("ex0", "data_processor.py")
    if mod is None:
        r.skip("ex0", "ex0/data_processor.py not found")
        return r.total()

    print(f"\n{DIM}  ex0/data_processor.py{OFF}")
    for fn in (section_validate, section_ingest, section_atomic,
               section_output, section_contract, section_routing,
               section_program):
        try:
            fn(r, mod)
        except Exception as exc:
            r.crash(exc)

    # ex1 reuses every ex0 class, so the whole ex0 suite must still pass
    # against data_stream.py - a copy-paste that drifted shows up here.
    mod1 = load("ex1", "data_stream.py")
    if mod1 is None:
        r.skip("ex1/data_stream.py", "not written yet")
    else:
        print(f"\n{DIM}  ex1/data_stream.py{OFF}")
        for fn in (section_validate, section_ingest, section_atomic,
                   section_output, section_contract, section_routing,
                   section_stream, section_program):
            try:
                fn(r, mod1)
            except Exception as exc:
                r.crash(exc)

    # ex2 carries every ex0 class AND DataStream, so the whole suite runs
    # again on top of the two plugin-specific sections.
    mod2 = load("ex2", "data_pipeline.py")
    if mod2 is None:
        r.skip("ex2/data_pipeline.py", "not written yet")
    else:
        print(f"\n{DIM}  ex2/data_pipeline.py{OFF}")
        for fn in (section_validate, section_ingest, section_atomic,
                   section_output, section_contract, section_routing,
                   section_stream, section_plugins, section_pipeline,
                   section_program):
            try:
                fn(r, mod2)
            except Exception as exc:
                r.crash(exc)

    return r.total()


if __name__ == "__main__":
    sys.exit(main())
