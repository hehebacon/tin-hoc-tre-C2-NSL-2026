#!/usr/bin/env python3
"""Validate the proposed waste image dataset layout without third-party packages."""
from pathlib import Path
import sys

SPLITS = ("train", "val", "test")
CLASSES = ("paper", "plastic", "metal", "other")
EXTENSIONS = {".jpg", ".jpeg", ".png", ".bmp", ".webp"}

def main() -> int:
    root = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("datasets/waste")
    errors = []
    total = 0
    for split in SPLITS:
        for label in CLASSES:
            folder = root / split / label
            if not folder.is_dir():
                errors.append(f"Missing directory: {folder}")
                continue
            images = [p for p in folder.rglob("*") if p.is_file() and p.suffix.lower() in EXTENSIONS]
            total += len(images)
            print(f"{split:5} {label:8} {len(images):5} images")
            if not images:
                errors.append(f"No images found in: {folder}")
    if errors:
        print("\nDataset check FAILED:")
        for error in errors:
            print(f"- {error}")
        return 1
    print(f"\nDataset layout OK: {total} image files found.")
    print("This checks folder layout only; it does not validate image quality or labels.")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
