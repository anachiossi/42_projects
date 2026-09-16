#! /usr/bin/env python3

"""Space station data validation with Pydantic.

Concepts learned:
 Basic data validation with Pydantic, including field constraints and
 error handling.
 BaseModel usage for defining structured data models.

Validates records from the Cosmic Data Observatory generated datasets:
reads space_stations.json (valid) and invalid_stations.json (rejected),
then reports the collected constraint violations for each.

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
space_stations.json and invalid_stations.json.

Run
---
    python3 space_station.py
"""

import json
import sys
from collections import defaultdict
from datetime import datetime
from pathlib import Path
from typing import Any

try:
    from pydantic import BaseModel, Field, ValidationError
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
TITLE: str = "Space Station Data Validation"
LINE: str = "=" * 40
VALID_TITLE: str = "Valid Stations"
INVALID_TITLE: str = "Invalid Stations"
VALID_HEADER: str = "Valid station created:"
ERROR_HEADER: str = "Expected validation error:"
STATUS_ON: str = "Operational"
STATUS_OFF: str = "Offline"

DATA_MSG: str = f"\nDownload the data generator from the project's page:\n\
{G}    data_generator.tar{Q}\n\n\
Extract data_generator.tar in repo's root:\n\
{Y}    tar -xf data_generator.tar{Q}\n\n\
Run the exporter:\n\
{Y}    python3 data_exporter.py{Q}\n\n\
This writes in ./generated_data/ the files needed:\n\
{G}    space_stations.json\n\
    invalid_stations.json{Q}\n\n\n\
After these steps rerun this script.\n"


# labels and units mirror show_station(), so both views read alike
LABELS: dict[str, tuple[str, str]] = {
    "station_id": ("ID", ""),
    "name": ("Name", ""),
    "crew_size": ("Crew", " people"),
    "power_level": ("Power", "%"),
    "oxygen_level": ("Oxygen", "%"),
    "last_maintenance": ("Maintenance", ""),
    "is_operational": ("Status", ""),
    "notes": ("Notes", ""),
}

# data comes from the observatory tools, not from this file
DATA_DIR: Path = Path(__file__).resolve().parent.parent / "generated_data"
VALID_FILE: Path = DATA_DIR / "space_stations.json"
INVALID_FILE: Path = DATA_DIR / "invalid_stations.json"


class SpaceStation(BaseModel):
    station_id: str = Field(..., min_length=3, max_length=10)
    name: str = Field(..., min_length=1, max_length=50)
    crew_size: int = Field(..., ge=1, le=20)
    power_level: float = Field(..., ge=0.0, le=100.0)
    oxygen_level: float = Field(..., ge=0.0, le=100.0)
    last_maintenance: datetime
    is_operational: bool = True
    notes: str | None = Field(default=None, max_length=200)


def show_station(station: SpaceStation) -> None:
    status: str = STATUS_ON if station.is_operational else STATUS_OFF
    print(f"{G}{VALID_HEADER}{Q}")
    print(f"ID: {station.station_id}")
    print(f"Name: {station.name}")
    print(f"Crew: {station.crew_size} people")
    print(f"Power: {station.power_level}%")
    print(f"Oxygen: {station.oxygen_level}%")
    print(f"Maintenance: {station.last_maintenance}")
    print(f"Status: {status}")
    print(f"Notes: {station.notes}")


def group_errors(error: ValidationError) -> dict[str, list[str]]:
    problems: defaultdict[str, list[str]] = defaultdict(list)
    for detail in error.errors():
        field = ".".join(map(str, detail["loc"])) or "model"
        problems[field].append(detail["msg"])
    return dict(problems)


def show_invalid(data: dict[str, Any], error: ValidationError) -> None:
    problems: dict[str, list[str]] = group_errors(error)

    print(f"{R}{ERROR_HEADER}{Q}")
    for key, value in data.items():
        name, unit = LABELS.get(key, (key, ""))
        messages: list[str] = problems.pop(key, [])
        if not messages:
            print(f"{name}: {value}{unit}")
        else:
            for message in messages:
                print(f"{R}{name}: {value!r}{unit} {Q}({message})")

    for key, messages in problems.items():
        name, _ = LABELS.get(key, (key, ""))
        for message in messages:
            print(f"{R}{name}: {Q}({message})")


def load_stations(path: Path) -> list[dict[str, Any]]:
    try:
        with open(path, encoding="utf-8") as data_file:
            records: list[dict[str, Any]] = json.load(data_file)
    except (OSError, json.JSONDecodeError) as error:
        print(f"{R}Could not read {path.name}: {error}{Q}")
        return []
    return records


def validate_station(data: dict[str, Any]) -> None:
    try:
        station: SpaceStation = SpaceStation(**data)
    except ValidationError as error:
        show_invalid(data, error)
    else:
        show_station(station)


def show_all(records: list[dict[str, Any]]) -> None:
    for index, data in enumerate(records):
        if index:
            print()
        validate_station(data)


def main() -> None:
    valid: list[dict[str, Any]] = load_stations(VALID_FILE)
    invalid: list[dict[str, Any]] = load_stations(INVALID_FILE)
    if not valid and not invalid:
        print(f"{DATA_MSG}")
        return
    print()
    print(f"{TITLE}\n")
    print(f"{LINE}")
    print(f"{VALID_TITLE}")
    print(f"{LINE}\n")
    show_all(valid)
    print()
    print(f"{LINE}")
    print(f"{INVALID_TITLE}")
    print(f"{LINE}\n")
    show_all(invalid)


if __name__ == "__main__":
    main()
