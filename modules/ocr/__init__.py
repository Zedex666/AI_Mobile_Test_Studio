# modules/ocr/__init__.py
# OCR module for text recognition

from typing import List, Dict
import pytesseract
from PIL import Image

class OCREngine:
    def __init__(self, lang: str = 'chi_sim+eng'):
        self.lang = lang

    def recognize(self, image_path: str) -> str:
        image = Image.open(image_path)
        return pytesseract.image_to_string(image, lang=self.lang)

    def recognize_boxes(self, image_path: str) -> List[Dict]:
        image = Image.open(image_path)
        data = pytesseract.image_to_data(image, lang=self.lang, output_type=pytesseract.Output.DICT)
        boxes = []
        for i in range(len(data['text'])):
            if int(data['conf'][i]) > 60:
                boxes.append({
                    'text': data['text'][i],
                    'x': data['left'][i],
                    'y': data['top'][i],
                    'width': data['width'][i],
                    'height': data['height'][i],
                    'confidence': data['conf'][i]
                })
        return boxes
