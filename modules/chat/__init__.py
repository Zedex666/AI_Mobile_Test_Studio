# modules/chat/__init__.py
# Chat module for AI conversation handling

from typing import List, Dict, Optional
import json

class ChatSession:
    def __init__(self, session_id: str):
        self.session_id = session_id
        self.messages: List[Dict] = []
        self.context: Dict = {}

    def add_message(self, role: str, content: str, attachments: Optional[List[str]] = None):
        msg = {
            "role": role,
            "content": content,
            "attachments": attachments or [],
            "timestamp": __import__('datetime').datetime.now().isoformat()
        }
        self.messages.append(msg)
        return msg

    def to_prompt(self) -> str:
        return json.dumps(self.messages, ensure_ascii=False, indent=2)
