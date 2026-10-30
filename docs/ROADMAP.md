# ECO-SORT AI — Implementation Roadmap

> Roadmap for the Advanced demonstrator. Dates are intentionally expressed as phases/sprints because the team's exact work calendar and competition submission deadline must be confirmed. Re-plan against the official competition schedule.

## Delivery strategy

Build in vertical slices. First prove that an item can be detected, held, released, and physically routed with a deterministic test label. Then add the camera. Then add real offline inference. Finally add feedback, metrics, and the dashboard. Do not wait for the AI model before testing the chute, and do not build a dashboard before the physical cycle works.

## Milestones

| Phase | Work | Exit criteria / evidence |
|---|---|---|
| 0 — Freeze requirements | Read competition rules; confirm accepted waste categories, size envelope, safety constraints, demo format and judging criteria | Requirements checklist approved; label policy and measurable success criteria written |
| 1 — Design & mock-up | Draw top/side views; make a cardboard/foam-board chute mock-up; test one-item feed, inspection pocket, selector travel and bin arrangement | Mock-up routes representative objects without snagging; dimensions and risk list recorded |
| 2 — Hardware decision & bench bring-up | Select exact ESP32-S3 board, camera, servo, sensor and power supply; verify pin map and electrical levels | Bill of materials with datasheets; camera captures; servo moves safely with linkage removed; sensors read reliably |
| 3 — Mechanical + deterministic firmware | Build the chute; implement feed debounce, gate/selector control, timeouts, HOME, REVIEW and JAM states using fixed test labels | Repeated test cycles route objects; no duplicate cycles from sensor chatter; jam timeout works |
| 4 — Dataset collection | Define annotation rules; collect diverse physical examples; label/review; split by physical object into train/validation/test | Versioned dataset manifest, class counts, reviewed labels, leakage-resistant split |
| 5 — Model training & PC evaluation | Train compact classifier; inspect confusion matrix; evaluate difficult examples; select threshold on validation set | Reproducible training configuration and held-out evaluation report; known failure cases documented |
| 6 — Edge deployment | Export/quantize for selected runtime; integrate preprocessing and inference adapter; benchmark on exact board | Firmware builds; model loads; measured latency/memory; inference result matches PC pipeline within documented tolerance |
| 7 — Full integration | Connect model output to confidence gate and chute; add exit sensor and local event logging | End-to-end offline sorting; UNKNOWN/invalid outputs route to REVIEW; successful count requires exit feedback |
| 8 — Reliability & presentation | Run repeated trials, lighting/occlusion/material tests, fault injection, offline demonstration and documentation | Test report, demo script, wiring/mechanical diagrams, reproducible build steps, known-limitations list |

## Suggested sprint order

### Sprint A — Requirements and mechanical proof
- Confirm official category names and competition constraints.
- Make a simple chute mock-up before CAD or printing.
- Test representative dry, clean, crushed, soft, reflective, and mixed-material objects.
- Decide whether a separate REVIEW bin fits the footprint.
- Produce a dimensioned drawing only after the mock-up works.

### Sprint B — Electronics and deterministic cycle
- Select board/camera only after matching camera support and pin map.
- Verify camera capture independently.
- Verify sensor levels and debounce independently.
- Test servo with linkage removed and an appropriate external supply.
- Implement the full state machine with hard-coded test labels.
- Measure actual servo current and check supply stability.

### Sprint C — Dataset and model
- Publish annotation guide before bulk collection.
- Collect multiple objects and backgrounds; avoid duplicate-heavy datasets.
- Separate training and held-out test sets by physical item/source.
- Train a small model and inspect per-class errors.
- Check whether visual-only METAL recognition is adequate; if not, evaluate an inductive sensor as an additional signal.
- Export only after the runtime/operator compatibility is confirmed.

### Sprint D — Edge AI and integration
- Implement exact decode/resize/normalization used in training.
- Integrate the quantized model behind the classifier interface.
- Validate output semantics; never treat logits as calibrated confidence without conversion.
- Add configurable threshold and REVIEW path.
- Verify that Wi-Fi is not used for inference or classification.

### Sprint E — Reliability and judging evidence
- Run 100 consecutive camera capture trials.
- Run repeated physical sorting trials per class and publish the denominator.
- Test low light, shadows, dirty/crushed objects, partial occlusion, lookalikes, double feed, blocked chute, sensor stuck, camera failure and missing model.
- Confirm fault states do not claim a successful sort.
- Capture a short offline demo video and prepare a one-page results sheet.

## Team allocation for a 10-person group

Roles can overlap, but each deliverable should have one named owner.

| Workstream | People | Deliverables |
|---|---:|---|
| Project lead / systems integration | 1 | Requirements, interfaces, weekly integration, risk register, final demo |
| Mechanical design and build | 2 | Chute mock-up, CAD, bins, gate/selector, jam testing |
| AI dataset and annotation | 2 | Label guide, collection protocol, reviewed dataset, split manifest |
| Model training and evaluation | 1 | Training pipeline, confusion matrix, threshold selection, model artifact |
| Embedded firmware | 2 | ESP-IDF modules, state machine, camera/inference integration, logs |
| Electronics and test | 1 | Wiring, power, sensors, bench tests, safety checks |
| Documentation and demo | 1 | Build guide, diagrams, test evidence, presentation/demo script |

The project lead should review interfaces and test evidence, not become the only person who can build or debug the system.

## Priority backlog

### P0 — Must work
- [ ] Official competition requirements and category policy confirmed.
- [ ] Chute mock-up accepts one item at a time.
- [ ] Exact board/camera and pin map documented.
- [ ] Camera capture works on real hardware.
- [ ] Servo and sensor bench tests pass.
- [ ] Deterministic state machine routes test labels safely.
- [ ] Dataset is labelled and split without near-duplicate leakage.
- [ ] Real quantized model runs locally on the selected board.
- [ ] Unknown/low-confidence/error path does not perform normal routing.
- [ ] Exit feedback gates successful-sort counters.
- [ ] End-to-end test works with network disconnected.

### P1 — Advanced evidence
- [ ] Local dashboard shows state, counters, faults and model metadata.
- [ ] Per-class precision/recall/F1 and confusion matrix are published.
- [ ] Latency, memory and flash measurements are published.
- [ ] 100 consecutive camera captures pass.
- [ ] Repeated physical routing results are recorded by class.
- [ ] Fault recovery and difficult-condition tests are documented.

### P2 — Only after P0/P1
- [ ] Per-bin level sensors.
- [ ] Local CSV export and session history.
- [ ] Optional inductive metal sensor fusion after feasibility test.
- [ ] Improved enclosure, removable trays and presentation finish.

## Definition of done

The Advanced demonstrator is ready only when:
1. A clean build is reproducible from the documented ESP-IDF setup.
2. The selected camera and pin map are proven on the actual board.
3. The model is trained on the project's dataset and evaluated on held-out physical items.
4. Inference runs locally without internet.
5. Low-confidence, invalid, and failed inference outcomes cannot be mistaken for successful classifications.
6. The chute routes each configured class repeatedly and does not jam under the tested item set.
7. The exit sensor confirms passage before the system increments success counters.
8. The team can explain limitations, failure cases, measurements, and the contribution of each subsystem.

## Risk register

| Risk | Impact | Mitigation |
|---|---|---|
| Camera board/pinout mismatch | Camera never initializes or conflicts with GPIO | Choose exact board-camera pair first; verify manufacturer pin map before wiring |
| Visual ambiguity between materials | False sorting | Better annotation and lighting; publish per-class metrics; consider sensor fusion for metal |
| Model too large/slow | No real-time inference | Start with a compact classifier; benchmark early on target hardware; reduce input size/model complexity based on measurements |
| Servo stalls or supply dips | Reset, bad routing, damaged linkage | Separate regulated supply; test without linkage; mechanical stops; measure current |
| Two objects enter together | Incorrect label or jam | One-item feed geometry and debounce; detect/clear multi-feed |
| Item sticks in chute | False success or blocked next cycle | Smooth removable chute, exit sensor, timeout and JAM state |
| Schedule consumed by dashboard | Core device incomplete | Dashboard is P1; finish physical offline loop first |
| Dataset leakage / weak test set | Inflated accuracy | Split by physical object/source; preserve a held-out test set |

## Weekly review template

At each review, record:
- What was physically or technically completed?
- What evidence proves it (commit, test log, photo, measurement)?
- What is blocked, and who owns the next action?
- What decision is waiting on a datasheet or real measurement?
- What will be integrated on the next bench session?

A task is not complete because code was written; it is complete when its acceptance test passes and evidence is stored.
