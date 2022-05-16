# TODO for RapidEvent

### Highest priority

- [ ] Reduce smearing of origin vertex for prompt particles.
- [ ] Add ProbNN variables with boolean values.
- [ ] Add decaying particles.

### High priority

- [ ] **#SEGFAULT** when trying to delete anything in _main_. Seems to come
from the TSTring destructor.
- [ ] Find a way to test the type of every branch of the data ROOT files to
avoid hardcoded skip of "nEvent" in RapidSelect.
- [ ] _RapidSelect::SelectTrack_ Manage error when a branch name is ill-formed.

### Low priority

- [ ] Add a warning when user ask for parameters which are not present in
the TTree.
- [ ] Print more info in _RapidConfig_.
- [ ] Better progress

### Done [x]

- [x] **#BUG** _RapidTrack::SelectTrack_ if some parametres have a letter in
common (try tokenize the branch name and get third token)
