# TODO for RapidEvent

### Highest priority
- [ ] **#ERROR_HNDLG** _RapidSelect_ Find a way to test the type of every
    branch of the data ROOT files to avoid hardcoded skip of `nEvent`.

### Bugs
- [ ] **#SEGFAULT** _RapidNorm::ComputeNorm_ When the name of the normalisation
histogram does not correspond to the particle name.

### Optimization
- [ ] _RapidSelect_ Rewrite track selection to avoid code duplication and
try to optimize a little -> Get only one entry per decay.
    - [x] Deactivate unused branches.

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
- [ ] **#MISC** Change all `Ssiz_t` to `Size_t`

### Done :heavy_check_mark:

- [x] **#BUG** _RapidTrack::SelectTrack_ if some parametres have a letter in
common (try tokenize the branch name and get third token)
- [x] **#SEGFAULT** in _main_ when trying to delete anything. Seems to come
from the TSTring destructor.
- [x] **#FEATURE** _main_ Better progress monitoring.
- [x] **#FEATURE** Reduce smearing of origin vertex for prompt particles.
- [x] **#FEATURE** Add ProbNN variables with boolean values.
- [x] **#FEATURE** Add decaying particles.
