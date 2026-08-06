#!/usr/bin/env python3
"""Regenerate the README screenshots in assets/ from real cmetal output.

Runs the built binary inside a fresh `cmetal init` workspace under a
pty, captures the ANSI output, and renders it to the SVG "terminal
window" format the README embeds:

  - assets/cmetal-1.svg  the watch-mode welcome screen
  - assets/cmetal-2.svg  a run-fail-hint session on intro1

Run it whenever the captured text changes (welcome message, command
output, version footer):

    cargo build --release
    python3 scripts/render_screenshots.py
"""

import os
import pty
import re
import select
import subprocess
import sys
import tempfile
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(REPO, "target", "release", "cmetal")

# The SVG terminal-window theme. CHAR_W matches the tspan x offsets of
# the previous generation, so regenerated files diff minimally.
BG = "#1b1e28"
DEFAULT = "#c8c8c8"
CHAR_W = 8.45
LINE_H = 21
X0, Y0 = 20.0, 56
FONT = "SF Mono, DejaVu Sans Mono, Menlo, monospace"

# 256-color SGR codes the binary emits, flattened to the theme's
# palette; anything unmapped renders in the default foreground.
COLORS = {
    9: "#f2777a", 1: "#f2777a", 31: "#f2777a",     # red
    10: "#99cc99", 2: "#99cc99", 32: "#99cc99",    # green
    11: "#ffcc66", 3: "#ffcc66", 33: "#ffcc66",    # yellow
}

SGR_RE = re.compile(r"\x1b\[([0-9;]*)m")
OTHER_ESC_RE = re.compile(r"\x1b\[[0-9;?]*[A-LN-Za-ln-z]|\x1b[()][0-9A-B]")
CLEAR_RE = re.compile(r"\x1b\[2J|\x1b\[H\x1b\[J|\x1bc")


def capture(argv, cwd, keys=b"", settle=1.0):
    """Run argv under a pty, optionally send keys, return raw output."""
    master, slave = pty.openpty()
    proc = subprocess.Popen(argv, cwd=cwd, stdin=slave, stdout=slave,
                            stderr=slave, close_fds=True)
    os.close(slave)
    out = b""
    sent = False
    quiet_since = time.monotonic()
    while True:
        r, _, _ = select.select([master], [], [], 0.2)
        if r:
            try:
                chunk = os.read(master, 65536)
            except OSError:
                break
            if not chunk:
                break
            out += chunk
            quiet_since = time.monotonic()
        elif proc.poll() is not None:
            break
        elif keys and not sent and time.monotonic() - quiet_since > settle:
            os.write(master, keys)
            sent = True
        elif time.monotonic() - quiet_since > 5:
            proc.terminate()
            break
    os.close(master)
    proc.wait()
    return out.decode("utf-8", errors="replace")


def to_spans(raw):
    """Parse ANSI text into lines of (color, bold, text) spans."""
    # Keep only what came after the last clear-screen: the final frame.
    raw = CLEAR_RE.split(raw)[-1]
    raw = OTHER_ESC_RE.sub("", raw)

    lines, spans, buf = [], [], ""
    color, bold = DEFAULT, False

    def flush():
        nonlocal buf
        if buf:
            spans.append((color, bold, buf))
            buf = ""

    pos = 0
    for m in SGR_RE.finditer(raw):
        text = raw[pos:m.start()]
        pos = m.end()
        for ch in text:
            if ch == "\n":
                flush()
                lines.append(spans[:])
                spans.clear()
            elif ch == "\r":
                pass
            else:
                buf += ch
        flush()
        params = [int(p) for p in m.group(1).split(";") if p] or [0]
        i = 0
        while i < len(params):
            p = params[i]
            if p == 0:
                color, bold = DEFAULT, False
            elif p == 1:
                bold = True
            elif p == 38 and params[i + 1:i + 2] == [5]:
                color = COLORS.get(params[i + 2], DEFAULT)
                i += 2
            elif 30 <= p <= 37:
                color = COLORS.get(p, DEFAULT)
            i += 1
    for ch in raw[pos:]:
        if ch == "\n":
            flush()
            lines.append(spans[:])
            spans.clear()
        elif ch != "\r":
            buf += ch
    flush()
    if spans:
        lines.append(spans[:])
    while lines and not any(s[2].strip() for s in lines[-1]):
        lines.pop()
    while lines and not any(s[2].strip() for s in lines[0]):
        lines.pop(0)
    return lines


def truncate_at(lines, marker):
    """Cut everything from the first line containing marker onward."""
    cut = next((i for i, line in enumerate(lines)
                if any(marker in s[2] for s in line)), len(lines))
    lines = lines[:cut]
    while lines and not any(s[2].strip() for s in lines[-1]):
        lines.pop()
    return lines


def render(lines, path):
    width = max((sum(len(s[2]) for s in line) for line in lines),
                default=0)
    w = int(2 * X0 + CHAR_W * width) + 1
    h = Y0 + LINE_H * (len(lines) - 1) + 14
    out = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" '
           f'height="{h}" font-family="{FONT}" font-size="14">',
           f'<rect width="{w}" height="{h}" rx="10" fill="{BG}"/>',
           f'<circle cx="22" cy="22" r="6" fill="#f2777a"/>',
           f'<circle cx="42" cy="22" r="6" fill="#ffcc66"/>',
           f'<circle cx="62" cy="22" r="6" fill="#99cc99"/>']
    y = Y0
    for line in lines:
        parts, x = [], X0
        for color, bold, text in line:
            if text:
                esc = (text.replace("&", "&amp;").replace("<", "&lt;")
                       .replace(">", "&gt;").replace("'", "&#x27;"))
                weight = ' font-weight="bold"' if bold else ""
                parts.append(f'<tspan x="{round(x, 1)}" fill="{color}"'
                             f'{weight} xml:space="preserve">{esc}</tspan>')
            x += CHAR_W * len(text)
        out.append(f'<text y="{y}">{"".join(parts)}</text>')
        y += LINE_H
    out.append("</svg>")
    with open(path, "w") as f:
        f.write("\n".join(out) + "\n")
    print(f"  wrote {os.path.relpath(path, REPO)} "
          f"({len(lines)} lines, {w}x{h})")


def prompt_line(cmd):
    return [("#99cc99", True, "$ "), (DEFAULT, False, cmd)]


def main():
    if not os.path.exists(BIN):
        sys.exit("build first: cargo build --release")
    with tempfile.TemporaryDirectory() as tmp:
        ws = os.path.join(tmp, "ws")
        subprocess.run([BIN, "init", ws], check=True, capture_output=True)

        # Shot 1: the watch-mode welcome screen. The captured session
        # continues past "Press any key" into the first exercise (with
        # a throwaway tmp path in it); the screenshot stops before.
        welcome = truncate_at(to_spans(capture([BIN], ws, keys=b"q")),
                              "Press any key")
        render(welcome, os.path.join(REPO, "assets", "cmetal-1.svg"))

        # Shot 2: a run-fail-hint session on intro1, with shell prompts.
        run_out = to_spans(capture([BIN, "run", "intro1"], ws))
        hint_out = to_spans(capture([BIN, "hint", "intro1"], ws))
        lines = ([prompt_line("cmetal run intro1")] + run_out + [[]]
                 + [prompt_line("cmetal hint intro1")] + hint_out)
        render(lines, os.path.join(REPO, "assets", "cmetal-2.svg"))


if __name__ == "__main__":
    main()
