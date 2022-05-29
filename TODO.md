# TODO for RapidEvent

### Highest priority

- [ ] Add decaying particles.

### High priority

- [ ] _RapidPV_ Add different smearing techniques.
    - [ ] Check that smearing corresponds to VELO resolution.
    - [ ] _RapidConfig_ Add a way to select them in the event config file.
- [ ] _RapidSelect_ Find a way to test the type of every branch of the data
ROOT files to avoid hardcoded skip of `nEvent`.
- [ ] _RapidSelect::SelectTrack_ Manage error when a branch name is ill-formed.

### Low priority

- [ ] _RapidConfig_ Add a warning when user ask for parameters which are not
present in the TTree.
- [ ] _RapidConfig_ Print more info.
- [ ] Check every include.
    - [ ] _RapidEvent_
    - [ ] _RapidConfig_
    - [ ] _RapidNorm_
    - [ ] _RapidSelect_
    - [ ] _RapidTrack_
    - [x] _RapidWriter_

### Done :heavy_check_mark:

- [x] **#BUG** _RapidTrack::SelectTrack_ if some parametres have a letter in
common (try tokenize the branch name and get third token)
- [x] **#SEGFAULT** in _main_ when trying to delete anything. Seems to come
from the TSTring destructor.
- [x] _main_ Better progress monitoring.
- [x] Reduce smearing of origin vertex for prompt particles.
- [x] Add ProbNN variables with boolean values.
