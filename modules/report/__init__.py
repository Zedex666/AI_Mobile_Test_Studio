# modules/report/__init__.py
# Report generation module

from typing import Dict, List, Optional
from datetime import datetime
import json

class TestReport:
    def __init__(self, title: str):
        self.title = title
        self.created_at = datetime.now()
        self.steps: List[Dict] = []
        self.summary: Dict = {
            "total": 0,
            "passed": 0,
            "failed": 0,
            "skipped": 0
        }

    def add_step(self, name: str, status: str, screenshot: Optional[str] = None,
                 log: Optional[str] = None, duration_ms: int = 0):
        step = {
            "name": name,
            "status": status,
            "screenshot": screenshot,
            "log": log,
            "duration_ms": duration_ms,
            "timestamp": datetime.now().isoformat()
        }
        self.steps.append(step)
        self.summary["total"] += 1
        if status == "passed":
            self.summary["passed"] += 1
        elif status == "failed":
            self.summary["failed"] += 1
        else:
            self.summary["skipped"] += 1

    def to_html(self) -> str:
        # TODO: Generate HTML report
        return f"<html><body><h1>{self.title}</h1></body></html>"

    def to_markdown(self) -> str:
        # TODO: Generate Markdown report
        return f"# {self.title}\n\nTotal: {self.summary['total']}"
