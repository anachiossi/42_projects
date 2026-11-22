# STUB — targets are in place, but the program itself is not written yet.
# The rules below are the ones the subject requires, plus a test target.

PYTHON  := python3
MAIN    := a_maze_ing.py
CONFIG  := config.txt

# Everything runs inside a local virtualenv, so the machine's own
# Python is never touched and a checkout only needs "make".
VENV    := .venv
VPYTHON := $(VENV)/bin/python
# Written once the dependencies are in: the other targets depend on it,
# so they install what they need the first time and skip it afterwards.
STAMP   := $(VENV)/.installed

# Colours for the help screen. ESC is built with printf so the escape
# character is stored literally and any echo prints it as it is.
ESC     := $(shell printf '\033')
BOLD    := $(ESC)[1m
DIM     := $(ESC)[2m
CYAN    := $(ESC)[36m
GREEN   := $(ESC)[32m
YELLOW  := $(ESC)[33m
RED     := $(ESC)[31m
RESET   := $(ESC)[0m

.PHONY: install run debug build clean fclean lint test help

# First target of the file, so a bare "make" lands here.
help:
	@printf '$(CYAN)$(BOLD)'
	@printf '%s\n' \
	'    ___         __  ___                       _' \
	'   /   |       /  |/  /___ _____  ___        (_)___  ____ _' \
	'  / /| |______/ /|_/ / __ `/_  / / _ \______/ / __ \/ __ `/' \
	' / ___ /_____/ /  / / /_/ / / /_/  __/_____/ / / / / /_/ /' \
	'/_/  |_|    /_/  /_/\__,_/ /___/\___/     /_/_/ /_/\__, /' \
	'                                                  /____/'
	@printf '$(RESET)$(DIM)%s$(RESET)\n\n' 'developed by anade-mo and ltamiano'
	@printf '$(GREEN)$(BOLD)%s$(RESET)\n' 'Run'
	@printf '  $(BOLD)%-12s$(RESET) %s\n' \
	'install' 'create $(VENV) and install the dependencies' \
	'run'     'draw the maze $(CONFIG) asks for' \
	'debug'   'run it under pdb' \
	'build'   'build the mazegen wheel at the repository root'
	@printf '\n$(YELLOW)$(BOLD)%s$(RESET)\n' 'Test'
	@printf '  $(BOLD)%-12s$(RESET) %s\n' \
	'test' 'run the unit tests' \
	'lint' 'flake8 + mypy --strict'
	@printf '\n$(RED)$(BOLD)%s$(RESET)\n' 'Clean'
	@printf '  $(BOLD)%-12s$(RESET) %s\n' \
	'clean'  'remove caches and build artefacts' \
	'fclean' 'clean, and remove $(VENV) as well'
	@printf '\n'

install: $(STAMP)

# Remade when pyproject.toml changes, since it is the only place the
# dependencies are declared. "-e ." installs the package itself in
# editable mode, "[dev]" adds the tools the checks need.
$(STAMP): pyproject.toml
	$(PYTHON) -m venv $(VENV)
	$(VPYTHON) -m pip install --upgrade pip
	$(VPYTHON) -m pip install -e .[dev]
	touch $(STAMP)

# The terminal is the renderer, so "run" is one rule again: it draws
# the maze from $(VENV), which any target here can build.
run: $(STAMP)
	@$(VPYTHON) $(MAIN) $(CONFIG) || true

debug: $(STAMP)
	$(VPYTHON) -m pdb $(MAIN) $(CONFIG)

# The reusable module, as the subject asks for it: a pip-installable
# file named mazegen-* sitting at the root of the repository. `python
# -m build` writes into dist/; the copy is what the defence installs.
build: $(STAMP)
	$(VPYTHON) -m pip install --quiet build
	$(VPYTHON) -m build
	cp dist/mazegen-*-py3-none-any.whl .
	cp dist/mazegen-*.tar.gz .

test: $(STAMP)
	$(VPYTHON) -m unittest discover -s tests -t .

# --strict is a superset of the five flags the subject requires of
# this rule; setup.cfg still names them one by one, so the connection
# to the subject stays visible where the flags actually live.
lint: $(STAMP)
	$(VPYTHON) -m flake8 .
	$(VPYTHON) -m mypy . --strict

clean:
	rm -rf .mypy_cache .pytest_cache build dist *.egg-info
	rm -f mazegen-*.whl mazegen-*.tar.gz
	find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true

# The virtualenv is rebuilt by any target that needs it, so removing it
# is safe; kept apart from clean because rebuilding costs a download.
fclean: clean
	rm -rf $(VENV)
