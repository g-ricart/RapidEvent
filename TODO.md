# TODO for RapidEvent

### Highest priority

- [ ] **#FEATURE** Add decaying particles.
    - [x] _RapidConfig_ Read config file.
    - [x] _RapidNorm_ Add decays.
    - [x] _RapidTrack_ Add track ID data member.
    - [x] _RapidTrack_ Add mother track ID data member.
    - [ ] _RapidSelect_ Add decays and IDs.
    - [ ] _RapidEvent_ Add decays and IDs
    - [ ] _RapidWriter_ Add IDs.
- [ ] **#ERROR_HNDLG** _RapidSelect_ Find a way to test the type of every
    branch of the data ROOT files to avoid hardcoded skip of `nEvent`.

### High priority
- [ ] **#FEATURE** Use MonteCarlo particle numbering scheme.
- [ ] **#FEATURE** _RapidPV_ Add different smearing techniques.
    - [ ] Check that smearing corresponds to VELO resolution.
    - [ ] _RapidConfig_ Add a way to select them in the event config file.
- [ ] **#ERROR_HNDLG** _RapidSelect::SelectTrack_ Manage error when a branch
name is ill-formed.
- [ ] **#FEATURE** Add possibility to have several decays of the same particle.

### Low priority

- [ ] **#ERROR_HNDLG** _RapidConfig_ Add a warning when user ask for parameters
which are not present in the TTree.
- [ ] **#MISC** _RapidConfig_ Print more info.
- [ ] **#MISC** Check every include.
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
- [x] **#FEATURE** _main_ Better progress monitoring.
- [x] **#FEATURE** Reduce smearing of origin vertex for prompt particles.
- [x] **#FEATURE** Add ProbNN variables with boolean values.
