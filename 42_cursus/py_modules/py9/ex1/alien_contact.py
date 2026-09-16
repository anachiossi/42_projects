#! /usr/bin/env python3

"""Alien contact data validation with Pydantic.

Concepts learned:
 Pydantic validators for rules that span more than one field.
 Enum-backed fields for values restricted to a fixed set.
 Separating valid from rejected records and explaining why each was
 rejected.

Validates records from the Cosmic Data Observatory generated datasets:
reads alien_contacts.json (valid) and invalid_contacts.json (rejected),
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
alien_contacts.json and invalid_contacts.json.

Run
---
    python3 alien_contact.py
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
TITLE: str = "Alien Contact Log Validation"
LINE: str = "=" * 38
VALID_HEADER: str = "Valid contact report:"
ERROR_HEADER: str = "Expected validation error:"
DATA_MSG: str = f"\nDownload the data generator from the project's page:\n\
{G}    data_generator.tar{Q}\n\n\
Extract data_generator.tar in repo's root:\n\
{Y}    tar -xf data_generator.tar{Q}\n\n\
Run the exporter:\n\
{Y}    python3 data_exporter.py{Q}\n\n\
This writes in ./generated_data/ the files needed:\n\
{G}    alien_contacts.json\n\
    invalid_contacts.json{Q}\n\n\n\
After these steps rerun this script.\n"

# labels and units mirror show_contact(), so both views read alike
LABELS: dict[str, tuple[str, str]] = {
    "contact_id": ("ID", ""),
    "timestamp": ("Timestamp", ""),
    "location": ("Location", ""),
    "contact_type": ("Type", ""),
    "signal_strength": ("Signal", "/10"),
    "duration_minutes": ("Duration", " minutes"),
    "witness_count": ("Witnesses", ""),
    "message_received": ("Message", ""),
    "is_verified": ("Verified", ""),
}

# data comes from the observatory tools, not from this file
DATA_DIR: Path = Path(__file__).resolve().parent.parent / "generated_data"
VALID_FILE: Path = DATA_DIR / "alien_contacts.json"
INVALID_FILE: Path = DATA_DIR / "invalid_contacts.json"


class ContactType(str, Enum):
    # Enum restricts contact_type to a set of valid values
    radio = "radio"
    visual = "visual"
    physical = "physical"
    telepathic = "telepathic"


class AlienContact(BaseModel):
    # field-level validation through Pydantic's Field constraints
    contact_id: str = Field(..., min_length=5, max_length=15)
    timestamp: datetime
    location: str = Field(..., min_length=3, max_length=100)
    contact_type: ContactType
    signal_strength: float = Field(..., ge=0.0, le=10.0)
    duration_minutes: int = Field(..., ge=1, le=1440)
    witness_count: int = Field(..., ge=1, le=100)
    message_received: str | None = Field(default=None, max_length=500)
    is_verified: bool = False

    # model-level validation runs after field-level validation, hence
    # mode="after": it checks additional business rules
    @model_validator(mode="after")
    def check_business_rules(self) -> "AlienContact":
        if not self.contact_id.startswith("AC"):
            raise ValueError("Contact ID must start with 'AC'")
        physical: bool = self.contact_type == ContactType.physical
        if physical and not self.is_verified:
            raise ValueError("Physical contact reports must be verified")
        telepathic: bool = self.contact_type == ContactType.telepathic
        if telepathic and self.witness_count < 3:
            raise ValueError(
                "Telepathic contact requires at least 3 witnesses")
        if self.signal_strength > 7.0 and not self.message_received:
            raise ValueError(
                "Strong signals (> 7.0) should include received messages")
        return self


def show_contact(contact: AlienContact) -> None:
    print(f"{G}{VALID_HEADER}{Q}")
    print(f"ID: {contact.contact_id}")
    print(f"Timestamp: {contact.timestamp}")
    print(f"Type: {contact.contact_type.value}")
    print(f"Location: {contact.location}")
    print(f"Signal: {contact.signal_strength}/10")
    print(f"Duration: {contact.duration_minutes} minutes")
    print(f"Witnesses: {contact.witness_count}")
    print(f"Message: {contact.message_received}")
    print(f"Verified: {contact.is_verified}")


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


def load_contacts(path: Path) -> list[dict[str, Any]]:
    try:
        with open(path, encoding="utf-8") as data_file:
            records: list[dict[str, Any]] = json.load(data_file)
    except (OSError, json.JSONDecodeError) as error:
        print(f"{R}Could not read {path.name}: {error}{Q}")
        return []
    return records


def validate_contact(data: dict[str, Any]) -> None:
    try:
        contact: AlienContact = AlienContact(**data)
    except ValidationError as error:
        show_invalid(data, error)
    else:
        show_contact(contact)


def show_all(records: list[dict[str, Any]]) -> None:
    for index, data in enumerate(records):
        if index:
            print()
        validate_contact(data)


def main() -> None:
    valid: list[dict[str, Any]] = load_contacts(VALID_FILE)
    invalid: list[dict[str, Any]] = load_contacts(INVALID_FILE)
    if not valid and not invalid:
        print(f"{DATA_MSG}")
        return
    print(TITLE)
    print(LINE)
    show_all(valid)
    print()
    print(LINE)
    show_all(invalid)


if __name__ == "__main__":
    main()
