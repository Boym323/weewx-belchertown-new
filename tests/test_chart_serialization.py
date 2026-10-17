"""Offline structural and serialization regression tests (no WeeWX installation required)."""
import ast
import json
import unittest
from pathlib import Path

SOURCE = Path("bin/user/belchertown.py")


class ChartSerializationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tree = ast.parse(SOURCE.read_text(encoding="utf-8"))
        generator = next(
            node for node in cls.tree.body
            if isinstance(node, ast.ClassDef) and node.name == "HighchartsJsonGenerator"
        )
        cls.run_method = next(
            node for node in generator.body
            if isinstance(node, ast.FunctionDef) and node.name == "run"
        )

    def test_compact_chart_json_remains_equivalent(self):
        """The wire format can be compact without altering chart values."""
        sample = {
            "day": {
                "chart1": {
                    "series": {"outTemp": {"data": [[100, 23.4], [200, None]]}},
                    "options": {"title": "Teplota"}
                }
            }
        }
        compact = json.dumps(sample, separators=(",", ":"))
        pretty = json.dumps(sample, indent=4)
        self.assertEqual(json.loads(compact), json.loads(pretty))
        self.assertLess(len(compact), len(pretty))
        calls = [
            n for n in ast.walk(self.run_method)
            if isinstance(n, ast.Call)
            and isinstance(n.func, ast.Attribute)
            and n.func.attr == "dump"
        ]
        self.assertTrue(any(
            any(k.arg == "separators" for k in call.keywords)
            for call in calls
        ))

    def test_debug_snapshot_is_outside_chart_group_loop(self):
        """Writing the same graphs.json inside the group loop multiplies disk IO."""
        run = self.run_method
        group_loops = [
            n for n in run.body
            if isinstance(n, ast.For)
            and any(isinstance(x, ast.Name) and x.id == "chart_group" for x in ast.walk(n.target))
        ]
        self.assertEqual(len(group_loops), 1)
        self.assertFalse(any(
            isinstance(n, ast.Name) and n.id == "chart_json_filename"
            for n in ast.walk(group_loops[0])
        ))
        self.assertTrue(any(
            isinstance(n, ast.Name) and n.id == "chart_json_filename"
            for n in ast.walk(run)
        ))


if __name__ == "__main__":
    unittest.main()
