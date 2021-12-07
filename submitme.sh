#!/usr/bin/env bash

##################################################
# Program to help you submit stuff for this lab. #
# Just run ./submitme.sh                         #
# If you need to specify files, use club instead #
##################################################

set -euo pipefail
shopt -s nullglob

srcdir=$(dirname "$(readlink -f "$0")")
labname=$(basename "$srcdir")
declare -a sfiles=( splc.hpp splc.cpp ast.hpp ast.cpp spl.lpp spl.ypp frame.hpp )

cd "$srcdir"

tempdir=$(mktemp --tmpdir -d)
function cleanup {
  cd "$srcdir"
  rm -rf "$tempdir"
  return 0
}
trap cleanup EXIT

anyfiles=0
for f in "${sfiles[@]}"; do
  if [[ -e $f ]]; then
    cp "$f" "$tempdir"/
    anyfiles=1
  fi
done
if (( ! anyfiles )); then
  echo "ERROR: no files found to submit"
  exit 1
fi
cd "$tempdir"

function find_cmd {
  if command -v "$1" 2>/dev/null; then
    return 0
  fi
  shift
  for f; do
    if [[ -x $f ]]; then
      readlink -f "$f"
      return 0
    fi
  done
  return 1
}

function get_club {
  declare -g club
  if club=$(find_cmd club ~/bin/club ~/.club/club); then
    return 0
  fi
  echo "club program not found; attempting to download it now"
  git clone "https://gitlab.usna.edu/roche/club.git" ~/.club
  mkdir -p ~/bin
  ln -s ../.club/club ~/bin/club
  club=$(find_cmd ~/bin/club)
  return 0
}

function submit_with_club {
  echo "using $club to submit..."
  "$club" -cSI413 -p"$labname" *
  cp .clubinfo "$srcdir"/
  exit 0
}

if get_club; then
  submit_with_club
elif submit=$(find_cmd submit ~/bin/submit); then
  echo "using $submit to submit..."
  submit -c=SI413 -p=$labname *
else
  echo <<EOF
ERROR: no way to submit.
Go download club at https://gitlab.usna.edu/roche/club#club
EOF
  exit 1
fi

:
