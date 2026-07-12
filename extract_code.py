#!/usr/bin/env python3
import re
import os
from pathlib import Path

TEX_DIR = Path("/Users/csv610/Projects/Courses/SahniBook/new-edition/tex/chapters")
OUT_DIR = Path("/Users/csv610/Projects/Courses/SahniBook/new-edition/code")

OUT_DIR.mkdir(parents=True, exist_ok=True)

# Pattern to find lstlisting blocks
# Handles optional [language=C++] and captures content until \end{lstlisting}
PATTERN = re.compile(
    r'\\begin{lstlisting}(?:\[[^\]]*\])?\n(.*?)\n\\end{lstlisting}',
    re.DOTALL
)

def clean_code(code: str) -> str:
    """Clean up LaTeX artifacts from code."""
    # Remove LaTeX comments that are not C++ comments
    lines = code.split('\n')
    cleaned = []
    for line in lines:
        # Skip lines that are pure LaTeX artifacts
        if line.strip().startswith('%') and not line.strip().startswith('//'):
            continue
        cleaned.append(line)
    return '\n'.join(cleaned).strip()

def extract_from_file(tex_path: Path, chapter_name: str):
    content = tex_path.read_text(encoding='utf-8', errors='ignore')
    matches = PATTERN.findall(content)
    
    chapter_dir = OUT_DIR / chapter_name
    chapter_dir.mkdir(exist_ok=True)
    
    count = 0
    for i, match in enumerate(matches):
        code = clean_code(match)
        if not code.strip():
            continue
        
        # Try to detect if it's C++ or ASCII diagram
        is_cpp = any(kw in code for kw in ['#include', 'template', 'class ', 'struct ', 'int main', 'std::', 'cout', 'cin', 'vector', 'map', 'set', 'queue', 'stack', 'priority_queue', 'unique_ptr', 'shared_ptr', 'make_unique', 'make_shared', 'constexpr', 'auto ', 'nullptr', 'override', 'virtual '])
        
        ext = '.cpp' if is_cpp else '.txt'
        filename = f"{chapter_name}_{i+1:03d}{ext}"
        filepath = chapter_dir / filename
        filepath.write_text(code + '\n', encoding='utf-8')
        count += 1
    
    return count

def main():
    total = 0
    for tex_file in sorted(TEX_DIR.glob("ch*.tex")):
        chapter_name = tex_file.stem  # e.g., ch01, ch02
        count = extract_from_file(tex_file, chapter_name)
        total += count
        print(f"{chapter_name}: {count} files")
    
    # Also process appendices
    for tex_file in sorted(TEX_DIR.glob("appendices.tex")):
        count = extract_from_file(tex_file, "appendices")
        total += count
        print(f"appendices: {count} files")
    
    print(f"\nTotal: {total} code files extracted to {OUT_DIR}")

if __name__ == "__main__":
    main()