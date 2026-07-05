# modules/explorer/__init__.py
# File explorer module

import os
from typing import List, Dict

class FileExplorer:
    def __init__(self, root_path: str = "."):
        self.root_path = root_path

    def list_files(self, path: str = "") -> List[Dict]:
        full_path = os.path.join(self.root_path, path)
        items = []
        for entry in os.scandir(full_path):
            items.append({
                "name": entry.name,
                "path": entry.path,
                "is_dir": entry.is_dir(),
                "size": entry.stat().st_size if entry.is_file() else 0
            })
        return items

    def read_file(self, path: str) -> str:
        full_path = os.path.join(self.root_path, path)
        with open(full_path, 'r', encoding='utf-8') as f:
            return f.read()

    def write_file(self, path: str, content: str):
        full_path = os.path.join(self.root_path, path)
        os.makedirs(os.path.dirname(full_path), exist_ok=True)
        with open(full_path, 'w', encoding='utf-8') as f:
            f.write(content)
