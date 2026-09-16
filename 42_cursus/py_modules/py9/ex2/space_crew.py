#! /usr/bin/env python3

"""Space mission crew validation with Pydantic.

Concepts learned:
 Nested Pydantic models: a model whose field is a list of other models.
 model_validator for cross-field invariants a single Field constraint cannot
 express.
 Grouping validation errors by field to report every violation at once.

Validates records from the Cosmic Data Observatory generated datasets:
reads space_missions.json (valid) and reports the collected constraint
violations for each. The exporter ships no invalid mission set, so the
rejected records are defined here, in INVALID_MISSIONS.

Environment setup
-----------------
    python3 -m venv cosmos_env
    source cosmos_env/bin/activate
    pip install "pydantic>=2,<3"
    deactivate  # when done

Test data
---------
Extract data_generator.tar in py09-space/, then run the exporter:

    tar -xf data_generator.tar
    python3 data_exporter.py

This writes ./generated_data/ (relative to the current directory) with
space_missions.json.

Run
---
    python3 space_crew.py
"""

import json
import sys
from collections import defaultdict
from datetime import datetime
from enum import Enum
from pathlib import Path
from typing import Any

try:
    from pydantic import BaseModel, Field, ValidationError, model_validator
except ImportError:
    sys.exit(
        "pydantic is required. Set up the environment first:\n"
        "    python3 -m venv cosmos_env\n"
        "    source cosmos_env/bin/activate\n"
        "    pip install 'pydantic>=2,<3'")

# in a terminal, colorize output if not on Windows:
COLOR: bool = sys.stdout.isatty() and not sys.platform.startswith("win")
G: str = "\033[32m" if COLOR else ""
R: str = "\033[31m" if COLOR else ""
Q: str = "\033[0m" if COLOR else ""
Y: str = "\033[33m" if COLOR else ""

# constants for output formatting
TITLE: str = "Space Mission Crew Validation"
LINE: str = "=" * 41
VALID_HEADER: str = "Valid mission created:"
CREW_HEADER: str = "Crew members:"
ERROR_HEADER: str = "Expected validation error:"
CREW_KEY: str = "crew"
DATA_MSG: str = f"\nDownload the data generator from the project's page:\n\
{G}    data_generator.tar{Q}\n\n\
Extract data_generator.tar in repo's root:\n\
{Y}    tar -xf data_generator.tar{Q}\n\n\
Run the exporter:\n\
{Y}    python3 data_exporter.py{Q}\n\n\
This writes in ./generated_data/ the file needed:\n\
{G}    space_missions.json{Q}\n\n\
After these steps rerun this script.\n"


# labels and units mirror show_mission(), so both views read alike
LABELS: dict[str, tuple[str, str]] = {
    "mission_id": ("ID", ""),
    "mission_name": ("Mission", ""),
    "destination": ("Destination", ""),
    "launch_date": ("Launch", ""),
    "duration_days": ("Duration", " days"),
    "crew": ("Crew size", ""),
    "mission_status": ("Status", ""),
    "budget_millions": ("Budget", "M"),
}

# data comes from the observatory tools, not from this file
DATA_DIR: Path = Path(__file__).resolve().parent.parent / "generated_data"
VALID_FILE: Path = DATA_DIR / "space_missions.json"


class Rank(str, Enum):
    # field-level validation through Pydantic's Enum
    cadet = "cadet"
    officer = "officer"
    lieutenant = "lieutenant"
    captain = "captain"
    commander = "commander"


class CrewMember(BaseModel):
    # field-level validation through Pydantic's Field constraints
    member_id: str = Field(..., min_length=3, max_length=10)
    name: str = Field(..., min_length=2, max_length=50)
    rank: Rank
    age: int = Field(..., ge=18, le=80)
    specialization: str = Field(..., min_length=3, max_length=30)
    years_experience: int = Field(..., ge=0, le=50)
    is_active: bool = True


class SpaceMission(BaseModel):
    # field-level validation through Pydantic's Field constraints
    mission_id: str = Field(..., min_length=5, max_length=15)
    mission_name: str = Field(..., min_length=3, max_length=100)
    destination: str = Field(..., min_length=3, max_length=50)
    launch_date: datetime
    duration_days: int = Field(..., ge=1, le=3650)
    crew: list[CrewMember] = Field(..., min_length=1, max_length=12)
    mission_status: str = "planned"
    budget_millions: float = Field(..., ge=1.0, le=10000.0)

    # model-level validation runs after field-level validation, hence
    # mode="after": it checks additional safety rules
    @model_validator(mode="after")
    def check_safety_rules(self) -> "SpaceMission":
        if not self.mission_id.startswith("M"):
            raise ValueError("Mission ID must start with 'M'")
        leaders = {Rank.commander, Rank.captain}
        if not any(member.rank in leaders for member in self.crew):
            raise ValueError(
                "Mission must have at least one Commander or Captain")
        if self.duration_days > 365:
            experienced = sum(
                member.years_experience >= 5 for member in self.crew)
            if experienced < len(self.crew) / 2:
                raise ValueError(
                    "Missions longer than 365 days require at least 50% "
                    "of crew with 5+ years experience")
        if not all(member.is_active for member in self.crew):
            raise ValueError("All crew members must be active")
        return self


# generated data does only produce valid missions data
# so we define some invalid missions for testing here:
INVALID_MISSIONS: list[dict[str, Any]] = [
    # every field is valid, so the model rules raise an error
    {
        "mission_id": "M2024_NOLEAD",
        "mission_name": "Leaderless Survey",
        "destination": "Ceres",
        "launch_date": "2024-06-01T00:00:00",
        "duration_days": 120,
        "crew": [
            {
                "member_id": "CM901",
                "name": "Nora Vance",
                "rank": "officer",  # no leader in the whole crew
                "age": 34,
                "specialization": "Navigation",
                "years_experience": 8,
                "is_active": True,
            },
        ],
        "mission_status": "planned",
        "budget_millions": 300.0,
    },
    # breaks four field constraints, so error messages are reported for each
    {
        "mission_id": "X2024_BADID",  # would break "starts with M",
        # but that rule is never reached
        "mission_name": "Malformed Mission",
        "destination": "Io",  # 2 chars, min_length is 3
        "launch_date": "2024-07-01T00:00:00",
        "duration_days": 4000,  # over the imposed maximum of 3650
        "crew": [
            {
                "member_id": "CM902",
                "name": "Ivo Reis",
                "rank": "commander",
                "age": 12,  # under the imposed minimum of 18
                "specialization": "Engineering",
                "years_experience": 2,
                "is_active": True,
            },
        ],
        "mission_status": "planned",
        "budget_millions": 20000.0,  # over the imposed maximum of 10000
    },
]


def show_crew_member(member: CrewMember) -> None:
    print(f"- {member.name} ({member.rank.value}) - {member.specialization}")


def show_mission(mission: SpaceMission) -> None:
    print(f"{G}{VALID_HEADER}{Q}")
    print(f"ID: {mission.mission_id}")
    print(f"Mission: {mission.mission_name}")
    print(f"Destination: {mission.destination}")
    print(f"Launch: {mission.launch_date}")
    print(f"Duration: {mission.duration_days} days")
    print(f"Status: {mission.mission_status}")
    print(f"Budget: {mission.budget_millions}M")
    print(f"Crew size: {len(mission.crew)}")
    print(CREW_HEADER)
    for member in mission.crew:
        show_crew_member(member)


def group_errors(error: ValidationError) -> dict[str, list[str]]:
    problems: defaultdict[str, list[str]] = defaultdict(list)
    for detail in error.errors():
        field = ".".join(map(str, detail["loc"])) or "model"
        message: str = detail["msg"].removeprefix("Value error, ")
        problems[field].append(message)
    return dict(problems)


def show_invalid(data: dict[str, Any], error: ValidationError) -> None:
    problems: dict[str, list[str]] = group_errors(error)

    print(f"{R}{ERROR_HEADER}{Q}")
    for key, value in data.items():
        name, unit = LABELS.get(key, (key, ""))
        # the crew is a list of records, print its size like show_mission
        shown: Any = len(value) if key == CREW_KEY else value
        messages: list[str] = problems.pop(key, [])
        if not messages:
            print(f"{name}: {shown}{unit}")
        else:
            for message in messages:
                print(f"{R}{name}: {shown!r}{unit} {Q}({message})")

    for key, messages in problems.items():
        name, _ = LABELS.get(key, (key, ""))
        for message in messages:
            print(f"{R}{name}: {Q}({message})")


def load_missions(path: Path) -> list[dict[str, Any]]:
    try:
        with open(path, encoding="utf-8") as data_file:
            records: list[dict[str, Any]] = json.load(data_file)
    except (OSError, json.JSONDecodeError) as error:
        print(f"{R}Could not read {path.name}: {error}{Q}")
        return []
    return records


def validate_mission(data: dict[str, Any]) -> None:
    try:
        mission: SpaceMission = SpaceMission(**data)
    except ValidationError as error:
        show_invalid(data, error)
    else:
        show_mission(mission)


def show_all(records: list[dict[str, Any]]) -> None:
    for index, data in enumerate(records):
        if index:
            print()
        validate_mission(data)


def main() -> None:
    valid: list[dict[str, Any]] = load_missions(VALID_FILE)
    if not valid:
        print(f"{DATA_MSG}")
        return
    print(TITLE)
    print(LINE)
    show_all(valid)
    print()
    print(LINE)
    show_all(INVALID_MISSIONS)


if __name__ == "__main__":
    main()
