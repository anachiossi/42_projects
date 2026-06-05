# 🎸 Get Next Line - The Stupid Test

Manual tests for `get_next_line` — karaoke mode and stdin mode.

📓 [Notion notes](https://square-green-2ba.notion.site/Get-Next-Line-37182782872c810194f7ffce6777f00c?source=copy_link)

---

## Files

| File | What it does |
|---|---|
| `tests/main_karaoke.c` | Reads a lyrics file, one lyric per Enter press |
| `tests/main_stdin.c` | Reads from keyboard input, prints each line back |
| `tests/main_quiz.c` | Bonus test — type the next lyric, get ✅ or ❌ |
| `tests/sing_along.txt` | Lyrics file — edit it and put whatever song you want |

---

## Setup

If you have your own `get_next_line`, just copy the `tests/` folder and the `Makefile` into your project root and run from there.

`tests/sing_along.txt` comes with a song already — just open it and replace the lyrics with whatever you want:

```bash
vim tests/sing_along.txt
```

Then build all three test executables at once:

```bash
make test
```

---

## Test 1 — Karaoke 🎤

```bash
./tests/karaoke
```

Press Enter to reveal each line. No it does not play the audio, haha, open youtube if you want the full experience.

---

## Test 2 — Stdin 🎹

```bash
./tests/stdin
```

Type lines in the terminal, press Enter after each. `Ctrl+D` to stop (sends EOF).

---

## Test 3 — Quiz 🎯 (bonus test)

Uses `get_next_line_bonus` with two fds at once — the lyrics file on one fd, stdin on the other. The real bonus multiple FDs as a game.

Put any music you want into `sing_along.txt`, change the titles to your new song, have fun.

```bash
./tests/quiz
```

Type the next lyric when prompted. `Ctrl+D` to quit.

---
