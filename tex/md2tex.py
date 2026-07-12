#!/usr/bin/env python3
"""Convert markdown chapters to LaTeX."""

import re, sys, os

def convert_md_to_tex(text, chapter_label):
    lines = text.split('\n')
    out = []
    in_code = False
    in_table = False
    in_itemize = False
    in_enumerate = False
    code_lang = ''
    code_buf = []

    def close_enumerate():
        nonlocal in_enumerate
        if in_enumerate:
            out.append('\\end{enumerate}\n')
            in_enumerate = False

    def close_itemize():
        nonlocal in_itemize
        if in_itemize:
            out.append('\\end{itemize}\n')
            in_itemize = False

    for raw_line in lines:
        line = raw_line

        # Code block (may be indented)
        if line.strip().startswith('```'):
            if in_code:
                if in_code == 'tikz':
                    out.append('\\end{tikzpicture}\n')
                    out.append('\\end{figure}\n')
                else:
                    out.append('\\end{lstlisting}\n')
                in_code = False
                code_lang = ''
            else:
                code_lang = line[3:].strip()
                if code_lang.lower() in ('tikz',):
                    out.append('\\begin{figure}[ht]\n\\centering\n')
                    out.append('\\begin{tikzpicture}\n')
                    in_code = 'tikz'
                    continue
                in_code = True
                out.append('\\begin{lstlisting}')
                if code_lang.lower() in ('cpp', 'c++'):
                    out.append('[language=C++]\n')
                elif code_lang.lower() in ('python', 'py'):
                    out.append('[language=Python]\n')
                elif code_lang.lower() in ('java'):
                    out.append('[language=Java]\n')
                else:
                    out.append('\n')
            continue

        if in_code:
            # Escape special LaTeX chars within code (but lstlisting handles this)
            out.append(line + '\n')
            continue

        # Horizontal rule
        if line.strip() == '---':
            close_itemize()
            close_enumerate()
            if in_table:
                out.append('\\bottomrule\n\\end{tabular}\n\\end{center}\n')
                in_table = False
            out.append('\\bigskip\n')
            continue

        # Blank line
        if line.strip() == '':
            close_itemize()
            close_enumerate()
            out.append('\n')
            continue

        # Table (simple heuristic: lines with | and --- separator)
        if re.match(r'^\|.*\|$', line.strip()):
            if re.match(r'^\|[\s\-:]+\|', line.strip()):
                continue  # skip separator row
            cells = [c.strip() for c in line.strip().split('|') if c.strip() != '']
            if not in_table:
                out.append('\\begin{center}\n\\begin{tabular}{' + 'c' * len(cells) + '}\n\\toprule\n')
                in_table = True
            cells_tex = [convert_inline(wrap_math_if_needed(c)) for c in cells]
            out.append(' & '.join(cells_tex) + ' \\\\\n')
            continue
        else:
            if in_table:
                out.append('\\bottomrule\n\\end{tabular}\n\\end{center}\n')
                in_table = False

        # Heading
        m = re.match(r'^(#{1,4})\s+(.+?)\s*$', line)
        if m:
            close_itemize()
            close_enumerate()
            level = len(m.group(1))
            raw_title = m.group(2)
            title = convert_inline(wrap_math_if_needed(raw_title))
            # Remove markdown link syntax in headings
            title = re.sub(r'\\href\{.+?\}\{(.+?)\}', r'\1', title)
            label_str = '\\label{' + chapter_label + ':' + slugify(raw_title) + '}'
            if level == 1:
                out.append('\\chapter{' + title + '}' + label_str + '\n')
            elif level == 2:
                out.append('\\section{' + title + '}\n')
            elif level == 3:
                out.append('\\subsection{' + title + '}\n')
            elif level == 4:
                out.append('\\subsubsection{' + title + '}\n')
            continue

        # Itemize (starts with - )
        if re.match(r'^\s*-\s+', line):
            close_enumerate()
            if not in_itemize:
                out.append('\\begin{itemize}\n')
                in_itemize = True
            item_text = re.sub(r'^\s*-\s+', '', line)
            out.append('\\item ' + convert_inline(wrap_math_if_needed(item_text)) + '\n')
            continue
        else:
            close_itemize()

        # Enumerate (starts with number.)
        if re.match(r'^\s*\d+[.)]\s+', line):
            close_itemize()
            if not in_enumerate:
                out.append('\\begin{enumerate}\n')
                in_enumerate = True
            item_text = re.sub(r'^\s*\d+[.)]\s+', '', line)
            out.append('\\item ' + convert_inline(wrap_math_if_needed(item_text)) + '\n')
            continue
        else:
            close_enumerate()

        # Regular paragraph
        if line.strip():
            out.append(convert_inline(wrap_math_if_needed(line)) + '\n')
        else:
            out.append('\n')

    close_itemize()
    close_enumerate()
    if in_table:
        out.append('\\bottomrule\n\\end{tabular}\n\\end{center}\n')

    return ''.join(out)

def escape_tex(text):
    """Escape LaTeX special characters."""
    text = text.replace('\\', '\\textbackslash{}')
    text = text.replace('{', '\\{').replace('}', '\\}')
    text = text.replace('$', '\\$')
    text = text.replace('#', '\\#')
    text = text.replace('%', '\\%')
    text = text.replace('&', '\\&')
    text = text.replace('_', '\\_')
    text = text.replace('~', '\\textasciitilde{}')
    text = text.replace('^', '\\textasciicircum{}')
    return text

# Unicode math char maps: char -> plain text for sub/sup conversion
UNICODE_SUB = {
    '₀': '0', '₁': '1', '₂': '2', '₃': '3', '₄': '4',
    '₅': '5', '₆': '6', '₇': '7', '₈': '8', '₉': '9',
    '₊': '+', '₋': '-', 'ₐ': 'a', 'ₑ': 'e', 'ₒ': 'o', 'ₓ': 'x',
}

UNICODE_SUP = {
    '⁰': '0', '¹': '1', '²': '2', '³': '3', '⁴': '4',
    '⁵': '5', '⁶': '6', '⁷': '7', '⁸': '8', '⁹': '9',
    '⁺': '+', '⁻': '-', '⁼': '=', 'ⁿ': 'n',
}

UNICODE_MATH_SYMS = {
    '⌊': r'\lfloor ', '⌋': r'\rfloor ',
    '⌈': r'\lceil ', '⌉': r'\rceil ',
    '→': r'\to ', '←': r'\leftarrow ', '↔': r'\leftrightarrow ',
    '⇒': r'\Rightarrow ', '⇐': r'\Leftarrow ',
    '≤': r'\le ', '≥': r'\ge ', '≠': r'\ne ',
    '≈': r'\approx ', '≡': r'\equiv ',
    'α': r'\alpha ', 'β': r'\beta ', 'γ': r'\gamma ', 'δ': r'\delta ',
    'ε': r'\varepsilon ', 'ζ': r'\zeta ', 'η': r'\eta ', 'θ': r'\theta ',
    'λ': r'\lambda ', 'μ': r'\mu ', 'ν': r'\nu ', 'ξ': r'\xi ',
    'π': r'\pi ', 'ρ': r'\rho ', 'σ': r'\sigma ', 'τ': r'\tau ',
    'φ': r'\phi ', 'χ': r'\chi ', 'ψ': r'\psi ', 'ω': r'\omega ',
    'Σ': r'\Sigma ', 'Π': r'\Pi ', 'Θ': r'\Theta ', 'Ω': r'\Omega ',
    '∞': r'\infty ', '∂': r'\partial ', '∇': r'\nabla ',
    '∀': r'\forall ', '∃': r'\exists ', '∅': r'\emptyset ',
    '∈': r'\in ', '∉': r'\notin ', '∋': r'\ni ',
    '⊂': r'\subset ', '⊃': r'\supset ', '⊆': r'\subseteq ', '⊇': r'\supseteq ',
    '∪': r'\cup ', '∩': r'\cap ', '∧': r'\wedge ', '∨': r'\vee ',
    '⊕': r'\oplus ', '⊗': r'\otimes ',
    '√': r'\sqrt ', '∛': r'\sqrt[3] ',
    '∫': r'\int ', '∑': r'\sum ', '∏': r'\prod ',
    '×': r'\times ', '·': r'\cdot ', '±': r'\pm ', '÷': r'\div ',
    '°': r'^\circ ', '′': r'^\prime ', '″': r'^{\prime\prime}',
}

def has_unicode_math(text):
    for ch in text:
        if ch in UNICODE_SUB or ch in UNICODE_SUP or ch in UNICODE_MATH_SYMS:
            return True
    return False

def convert_unicode_math(text):
    """Convert Unicode math chars within a math expression.

    Groups consecutive sub/superscript characters into single
    _{...} or ^{...} spans.
    """
    result = []
    i = 0
    while i < len(text):
        ch = text[i]

        # Subscript run
        if ch in UNICODE_SUB:
            run = ''
            while i < len(text) and text[i] in UNICODE_SUB:
                run += UNICODE_SUB[text[i]]
                i += 1
            result.append('_{' + run + '}')
            continue

        # Superscript run
        if ch in UNICODE_SUP:
            run = ''
            while i < len(text) and text[i] in UNICODE_SUP:
                run += UNICODE_SUP[text[i]]
                i += 1
            result.append('^{' + run + '}')
            continue

        if ch in UNICODE_MATH_SYMS:
            replacement = UNICODE_MATH_SYMS[ch]
            result.append(replacement)
            i += 1
            continue

        result.append(ch)
        i += 1

    # Balance any unclosed \sqrt or \sqrt[3]{ braces
    out = ''.join(result)
    open_braces = out.count('{') - out.count('}')
    if open_braces > 0:
        out += '}' * open_braces
    return out

def wrap_math_if_needed(text):
    """Wrap math expressions in $...$ within text.

    Converts Unicode math characters to inline $...$ spans without
    wrapping entire paragraphs. For ^ exponents, wraps the expression.
    """
    # Already has math mode markers — skip
    if '$' in text:
        return text

    needs_math = has_unicode_math(text)

    # Check for ^ used as exponent (not escaped)
    if '^' in text and '\\textasciicircum' not in text and '\\^' not in text:
        if re.search(r'[\w)\]]\^[\w{]', text):
            needs_math = True

    if not needs_math:
        return text

    # Use inline conversion for individual math spans
    result = convert_unicode_math_inline(text)

    # Also handle ^ exponents that weren't converted by unicode math conversion
    result = re.sub(r'(\w)\^(\w)', r'$\1^{\2}$', result)

    return result

def convert_unicode_math_inline(text):
    """Convert Unicode math chars in mixed markup text.

    Each sub/superscript run gets its own $...$ span.
    Each math symbol gets its own $...$ span.
    """
    result = []
    i = 0
    n = len(text)

    while i < n:
        ch = text[i]

        # Subscript run
        if ch in UNICODE_SUB:
            run = ''
            while i < n and text[i] in UNICODE_SUB:
                run += UNICODE_SUB[text[i]]
                i += 1
            result.append('$_{' + run + '}$')
            continue

        # Superscript run
        if ch in UNICODE_SUP:
            run = ''
            while i < n and text[i] in UNICODE_SUP:
                run += UNICODE_SUP[text[i]]
                i += 1
            result.append('$^{' + run + '}$')
            continue

        # Math symbol
        if ch in UNICODE_MATH_SYMS:
            sym = UNICODE_MATH_SYMS[ch]
            i += 1
            # Special handling for sqrt: include the radicand in the math span
            if ch in ('√', '∛'):
                radicand = ''
                # Skip optional space
                while i < n and text[i] == ' ':
                    i += 1
                if i < n and text[i] in '({':
                    depth = 0
                    while i < n:
                        if text[i] in '({':
                            depth += 1
                            radicand += text[i]
                        elif text[i] in ')}':
                            depth -= 1
                            radicand += text[i]
                            if depth == 0:
                                i += 1
                                break
                        else:
                            radicand += text[i]
                        i += 1
                else:
                    while i < n and (text[i].isalnum() or text[i] in '+−'):
                        radicand += text[i]
                        i += 1
                result.append('$' + sym + '{' + radicand + '}$')
            else:
                result.append('$' + sym + '$')
            continue

        result.append(ch)
        i += 1

    return ''.join(result)

def escape_tt(text):
    """Escape for \\texttt context: only underscore (handled by T1 encoding),
    and ensure braces are balanced for template code."""
    # In \texttt, _ is the only special char that causes issues with T1 encoding
    # But we should be safe and escape it
    text = text.replace('\\', '\\textbackslash{}')
    text = text.replace('_', '\\_')
    text = text.replace('#', '\\#')
    text = text.replace('%', '\\%')
    text = text.replace('&', '\\&')
    text = text.replace('{', '\\{').replace('}', '\\}')
    text = text.replace('$', '\\$')
    text = text.replace('~', '\\textasciitilde{}')
    text = text.replace('^', '\\textasciicircum{}')
    return text

def convert_inline(text):
    """Convert inline markdown to LaTeX."""
    # Bold
    text = re.sub(r'\*\*(.+?)\*\*', r'\\textbf{\1}', text)
    # Italic
    text = re.sub(r'(?<!\*)\*(?!\*)(.+?)(?<!\*)\*(?!\*)', r'\\textit{\1}', text)
    # Inline code (must be escaped carefully)
    text = re.sub(r'`([^`]+)`', lambda m: '\\texttt{' + escape_tt(m.group(1)) + '}', text)
    # Links
    text = re.sub(r'\[(.+?)\]\((.+?)\)', r'\\href{\2}{\1}', text)
    # Escape remaining special chars (outside \texttt, \href, and math)
    result = []
    i = 0
    while i < len(text):
        if text[i:i+8] == '\\texttt{':
            depth = 1
            j = i + 8
            while j < len(text) and depth > 0:
                if text[j] == '{': depth += 1
                elif text[j] == '}': depth -= 1
                j += 1
            result.append(text[i:j])
            i = j
        elif text[i:i+6] == '\\href{':
            # skip over \href{url}{text}
            depth = 1
            j = i + 6
            while j < len(text) and depth > 0:
                if text[j] == '{': depth += 1
                elif text[j] == '}': depth -= 1
                j += 1
            # second brace pair for the link text
            depth = 1
            while j < len(text) and depth > 0:
                if text[j] == '{': depth += 1
                elif text[j] == '}': depth -= 1
                j += 1
            result.append(text[i:j])
            i = j
        elif i+1 < len(text) and text[i] == '\\' and text[i+1] == '$':
            result.append('\\$')
            i += 2
        elif text[i] == '$':
            j = text.find('$', i + 1)
            if j == -1:
                result.append('\\$')
                i += 1
            else:
                result.append(text[i:j+1])
                i = j + 1
        elif text[i] == '_':
            result.append('\\_')
            i += 1
        elif text[i] == '#':
            result.append('\\#')
            i += 1
        elif text[i] == '%':
            result.append('\\%')
            i += 1
        elif text[i] == '&':
            result.append('\\&')
            i += 1
        elif text[i] == '~':
            result.append('\\textasciitilde{}')
            i += 1
        elif text[i] == '^':
            result.append('\\textasciicircum{}')
            i += 1
        else:
            result.append(text[i])
            i += 1
    return ''.join(result)

def slugify(text):
    s = text.lower()
    s = re.sub(r'[^a-z0-9]+', '-', s)
    s = s.strip('-')
    return s

def convert_file(inpath, outpath, label):
    with open(inpath) as f:
        text = f.read()
    tex = convert_md_to_tex(text, label)
    with open(outpath, 'w') as f:
        f.write(tex)

if __name__ == '__main__':
    ch_dir = sys.argv[1] if len(sys.argv) > 1 else '../chapters'
    out_dir = sys.argv[2] if len(sys.argv) > 2 else 'chapters'
    os.makedirs(out_dir, exist_ok=True)

    chapters = [
        ('preface', 'preface'),
        ('ch01-modern-cpp-review', 'ch01'),
        ('ch02-performance-analysis', 'ch02'),
        ('ch03-performance-measurement', 'ch03'),
        ('ch04-linear-lists', 'ch04'),
        ('ch05-arrays-matrices-sparse', 'ch05'),
        ('ch06-stacks', 'ch06'),
        ('ch07-queues', 'ch07'),
        ('ch08-hashing', 'ch08'),
        ('ch09-trees', 'ch09'),
        ('ch10-priority-queues', 'ch10'),
        ('ch11-search-trees', 'ch11'),
        ('ch12-graphs', 'ch12'),
        ('ch13-strings-tries', 'ch13'),
        ('ch14-segment-fenwick-unionfind', 'ch14'),
        ('ch15-greedy', 'ch15'),
        ('ch16-divide-conquer', 'ch16'),
        ('ch17-dynamic-programming', 'ch17'),
        ('ch18-backtracking-branchbound', 'ch18'),
        ('ch19-probabilistic', 'ch19'),
        ('ch20-maxflow-matching', 'ch20'),
        ('appendices', 'appendices'),
    ]

    for src_name, dst_name in chapters:
        src = os.path.join(ch_dir, src_name + '.md')
        dst = os.path.join(out_dir, dst_name + '.tex')
        if os.path.exists(src):
            print(f'Converting {src_name}...')
            convert_file(src, dst, dst_name)
        else:
            print(f'Skipping {src_name} (not found)')

    print('Done.')
