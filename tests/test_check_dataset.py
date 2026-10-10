import tempfile
import unittest
import subprocess
import sys
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "tools" / "check_dataset.py"
SPLITS = ("train", "val", "test")
CLASSES = ("paper", "plastic", "metal", "other")

class DatasetLayoutTests(unittest.TestCase):
    def _make_layout(self, root, empty=None):
        for split in SPLITS:
            for label in CLASSES:
                folder = root / split / label
                folder.mkdir(parents=True)
                if (split, label) != empty:
                    (folder / "sample.jpg").write_bytes(b"placeholder")

    def test_valid_layout_passes(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._make_layout(root)
            result = subprocess.run([sys.executable, str(SCRIPT), str(root)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("Dataset layout OK", result.stdout)

    def test_empty_class_folder_fails(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._make_layout(root, empty=("test", "metal"))
            result = subprocess.run([sys.executable, str(SCRIPT), str(root)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 1)
            self.assertIn("No images found", result.stdout)

if __name__ == "__main__":
    unittest.main()
