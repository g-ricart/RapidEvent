# TODO for RapidEvent

### Highest priority

- [ ] #BUG in RapidTrack::SelectTrack() if some parametres have a letter in common (try tokenize the branch name and get third token)
- [ ] Add decaying particles
- [x] Finish implementing RapidWriter

### High priority

- [ ] Find a way to test the type of every branch of the data ROOT files to avoid hardcoded skip of "nEvent" in RapidSelect

### Low priority

- [ ] Add a warning when user ask for parameters which are not present in the TTree
