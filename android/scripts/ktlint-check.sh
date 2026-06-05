#!/usr/bin/env bash
set -e
SCRIPT_PATH="$(realpath $0)"
GRADLE_DIR="$(realpath "$(dirname ${SCRIPT_PATH})/../")"

# Pre-commit passes files as arguments, but ktlintCheck works on the whole project
# Just run the ktlint check task
"${GRADLE_DIR}"/gradlew -p "${GRADLE_DIR}" ktlintFormat --no-daemon
