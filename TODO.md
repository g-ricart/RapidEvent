# TODO for RapidEvent

### Highest priority

- [ ] Reduce smearing of origin vertex for prompt particles.
- [ ] Add ProbNN variables with boolean values.
- [ ] Add decaying particles.

### High priority

- [ ] _RapidSelect_ Find a way to test the type of every branch of the data ROOT files to
avoid hardcoded skip of "nEvent".
- [ ] _RapidSelect::SelectTrack_ Manage error when a branch name is ill-formed.

### Low priority

- [ ] _RapidConfig_ Add a warning when user ask for parameters which are not present in
the TTree.
- [ ] _RapidConfig_ Print more info.
- [ ] _main_ Better progress monitoring.

### Done [x]

- [x] **#BUG** _RapidTrack::SelectTrack_ if some parametres have a letter in
common (try tokenize the branch name and get third token)
- [x] **#SEGFAULT** in _main_ when trying to delete anything. Seems to come
from the TSTring destructor.
