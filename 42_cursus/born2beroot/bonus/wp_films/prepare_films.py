"""Prepare the portfolio films for the WordPress import (runs on WSL).

Steps:
    1. copy films.json from the website into ./package/
    2. convert each film's hero image from AVIF to JPG into
       ./package/images/ (WordPress on the VM may not accept AVIF)

To add a step: write one more function taking the package folder and
call it from main().
"""

import json
import shutil
from pathlib import Path

from PIL import Image

WEBSITE = Path("/mnt/d/website/assets")
PACKAGE = Path(__file__).parent / "package"
MAX_WIDTH = 1280
JPG_QUALITY = 82


def copy_films(package: Path) -> list[dict]:
    """Step 1: copy films.json and return its list of films."""
    source = WEBSITE / "films.json"
    shutil.copyfile(source, package / "films.json")
    with open(source, encoding="utf-8") as file:
        films: list[dict] = json.load(file)
    print(f"step 1: {len(films)} films copied")
    return films


def convert_images(package: Path, films: list[dict]) -> None:
    """Step 2: one JPG per film, named <image_id>.jpg, at most MAX_WIDTH wide."""
    images = package / "images"
    images.mkdir(exist_ok=True)
    converted = 0
    for film in films:
        source = WEBSITE / "images" / "heroes" / f"{film['image_id']}.avif"
        if not source.exists():
            print(f"step 2: no image for {film['title']}")
            continue
        with Image.open(source) as picture:
            rgb = picture.convert("RGB")
            if rgb.width > MAX_WIDTH:
                height = rgb.height * MAX_WIDTH // rgb.width
                rgb = rgb.resize((MAX_WIDTH, height))
            rgb.save(images / f"{film['image_id']}.jpg", quality=JPG_QUALITY)
        converted += 1
    print(f"step 2: {converted} images converted")


def main() -> None:
    PACKAGE.mkdir(exist_ok=True)
    films = copy_films(PACKAGE)
    convert_images(PACKAGE, films)
    shutil.copyfile(Path(__file__).parent / "import_films.sh",
                    PACKAGE / "import_films.sh")
    print(f"ready: {PACKAGE}")


if __name__ == "__main__":
    main()
