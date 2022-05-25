# TODO for RapidEvent

### Highest priority

- [ ] Reduce smearing of origin vertex for prompt particles.
    - [x] Add a `RapidPV` class to define and smear the primary vertex.
    - [x] _RapidTrack_  Add a way to specify if a track is prompt or not.
    - [x] _RapidConfig_ Differentiate between prompt and non-prompt particles.
    - [x] _RapidEvent_  Add a `RapidPV` data member.
    - [x] _RapidEvent_  Differentiate between prompt and non-prompt tracks.
    - [x] _RapidTrack_  Add PV smeared and true coordinates of prompt
    tracks to the track parameters if the track is prompt.
    - [ ] Check that smearing corresponds to VELO resolution.
- [ ] Add ProbNN variables with boolean values.
- [ ] Add decaying particles.

### High priority

- [ ] _RapidPV_ Add different smearing techniques.
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

### Done [x]

- [x] **#BUG** _RapidTrack::SelectTrack_ if some parametres have a letter in
common (try tokenize the branch name and get third token)
- [x] **#SEGFAULT** in _main_ when trying to delete anything. Seems to come
from the TSTring destructor.
- [x] _main_ Better progress monitoring.
