# modules/terminal/__init__.py
# Terminal module for command execution

import subprocess
import shlex
from typing import Optional, Callable

class TerminalSession:
    def __init__(self):
        self.history: list = []
        self.current_process: Optional[subprocess.Popen] = None

    def execute(self, command: str, cwd: Optional[str] = None,
                stdout_callback: Optional[Callable[[str], None]] = None,
                stderr_callback: Optional[Callable[[str], None]] = None):
        self.history.append(command)
        # TODO: Implement async execution with callbacks
        result = subprocess.run(shlex.split(command), capture_output=True, text=True, cwd=cwd)
        if stdout_callback and result.stdout:
            stdout_callback(result.stdout)
        if stderr_callback and result.stderr:
            stderr_callback(result.stderr)
        return result
