# ECO-SORT AI — Modular Mechanical Assembly Proposal

> Status: **design proposal for the first physical prototype**. This is not a dimensioned CAD drawing, verified bill of materials, or validated wiring plan. Measure the actual parts and build a cardboard/foam-board mock-up before ordering or fabricating the final frame.

## 1. Design intent

Use a compact, gravity-fed sorter built from removable modules. The item is presented in a fixed inspection chamber, classified locally, then released through a servo-positioned selector into one of four bins. Uncertain items are held for manual review or sent to a dedicated REVIEW container.

The mechanical design must prioritize:
- one item per cycle;
- repeatable camera distance, angle, background, and lighting;
- a clear path with no snagging corners;
- access to the camera, gate, servo, and wiring without dismantling the whole machine;
- safe behavior when power is lost or the classifier returns UNKNOWN.

## 2. Module breakdown

```text
                 [ M1 — FEED HOPPER ]
                          |
                          v
             [ M2 — SINGULATION / HOLD GATE ]
                          |
                          v
       +---------------------------------------+
       | M3 — INSPECTION CHAMBER               |
       |  fixed camera + diffused LED + tray   |
       +---------------------------------------+
                          |
                 [ M4 — RELEASE GATE ]
                          |
                          v
             [ M5 — SERVO ROUTING CHUTE ]
                    /   /   |   \
                   v   v    v    v
                 PAPER PLASTIC METAL OTHER
                          |
                    [ M6 — BIN BAY ]

       [ M7 — ELECTRONICS / POWER BAY ]
       ESP32-S3 + camera interface + sensor
       interface + servo driver + protected
       power distribution; mounted separately
       from removable waste-contact modules.
```

### M1 — Feed hopper
- Function: guide an item into the machine without allowing a large batch to fall in.
- Form: sloped walls leading to a narrow throat; removable top cover.
- Design notes: round or soften internal edges; make the throat wide enough for the largest representative item, then verify singulation with real samples.
- Do not set final width until the team defines the accepted item-size envelope.

### M2 — Singulation and hold gate
- Function: allow one item into the inspection area and prevent a second item following it.
- First prototype: a manually operated slide or simple servo gate is acceptable; keep the gate mechanically independent from the routing selector.
- Add an item-present sensor at a location that cannot be blocked by the gate itself.
- If two items enter together, stop the cycle and request manual clearing.

### M3 — Inspection chamber
- Function: hold the item still at a repeatable position for image capture.
- Parts: matte neutral background, fixed camera mount, diffused white LED lighting, removable tray, light shield.
- Camera mount should have adjustment slots for initial focus/framing, then lock securely.
- Keep reflective surfaces and direct LED glare out of the camera view.
- Provide a cable route with strain relief; do not let wires enter the item path.

### M4 — Release gate
- Function: retain the item while the camera and model run, then release only after a valid route is selected.
- Prefer a gravity-assisted flap or sliding gate with a physical stop.
- On reset or fault, the gate should remain in its safe holding position where mechanically possible.
- Do not use the release gate as the four-way selector; separate functions simplify calibration and fault diagnosis.

### M5 — Servo routing chute
- Function: align one outlet with the selected bin.
- First prototype: one pivoting flap/selector with four repeatable detents or measured positions.
- Add physical travel stops so the servo cannot force the mechanism beyond its range.
- Keep the linkage outside the falling-item channel.
- Calibrate positions with the servo disconnected from the linkage first; attach the linkage only after safe travel is confirmed.
- A failed or low-confidence prediction must not move the selector to a normal material bin.

### M6 — Removable bin bay
- Four removable containers: PAPER, PLASTIC, METAL, OTHER.
- Recommended: a separate REVIEW/UNKNOWN container. If space prevents this, route uncertain items to a safe manual-access position and count them separately as REVIEW; do not silently label them OTHER.
- Use keyed rails or simple stops so bins cannot be inserted under the wrong outlet.
- Add labels visible from the front and enough clearance to remove each bin without moving the electronics.

### M7 — Electronics and power bay
- Mount the ESP32-S3 board, sensor connectors, servo driver/interface, and power distribution on a removable plate.
- Keep the electronics bay away from loose waste and possible spills.
- Provide ventilation and access to USB/programming connector, reset, and power switch.
- Servo supply must be separate from the ESP32 3.3 V rail. For a non-isolated PWM signal, connect grounds together.
- Verify every sensor output voltage before connecting it to an ESP32 GPIO. Never apply 5 V logic directly to an ESP32 input.
- Use connectors and labels so the camera, sensors, servo, and power can be disconnected independently.

## 3. Suggested frame arrangement

Use a vertical stack to minimize footprint:

1. Hopper at the top.
2. Inspection chamber immediately below the hopper, with the camera looking into the chamber from a fixed side/top mount.
3. Release gate below the chamber.
4. Routing selector below the release gate.
5. Four-bin bay at the bottom.
6. Electronics bay on the rear or side, not beneath the waste path.

Keep the frame modular: a left/right structural side plate, removable inspection tray, removable selector assembly, and slide-out bin bay. Choose plywood, acrylic, or 3D-printed brackets only after a mock-up proves the geometry. Avoid committing to a full printed enclosure before the chute has passed feed tests.

## 4. Assembly order

### Stage A — cardboard geometry mock-up
1. Mark the maximum intended item size using actual sample waste.
2. Build the hopper, inspection pocket, release opening, selector, and bin layout from cardboard or foam board.
3. Test paper, plastic, metal, and OTHER examples in different orientations.
4. Check for bridging, snagging, bouncing, and items entering two-at-a-time.
5. Adjust the chute angles and openings; record measurements only after repeated successful trials.

### Stage B — frame and passive modules
1. Build the frame and bin rails.
2. Install the hopper and inspection chamber.
3. Install the removable tray and light shield.
4. Install the release gate without connecting a servo.
5. Install the selector and verify that it moves freely by hand.
6. Check that all bins can be removed independently.

### Stage C — electrical bring-up
1. Mount the controller and power distribution in the electronics bay.
2. Verify the selected board and camera pin map from their exact documentation.
3. Test sensors individually with a meter/logic-safe setup.
4. Test the servo using a separate supply with the linkage disconnected.
5. Confirm HOME and each route position without allowing mechanical collision.
6. Only then connect the servo linkage and repeat low-speed tests.

### Stage D — camera and lighting
1. Fix the camera mount and set focus/framing.
2. Install diffused lighting and the matte background.
3. Capture at least 100 consecutive frames and record capture failures.
4. Check that images remain consistent with different room lighting conditions.
5. Lock the camera and light mounts after the image region is stable.

### Stage E — integrated sorting
1. Run the state machine with the classifier forced to UNKNOWN; verify that normal routing never occurs.
2. Test each route using a known safe test item and a manually selected route.
3. Integrate the trained model only after its label order, input preprocessing, score semantics, and threshold are documented.
4. Verify exit feedback and timeout behavior.
5. Disconnect network access and repeat the complete demonstration.

## 5. Interface and control contract

| Interface | Input/output | Required behavior |
|---|---|---|
| Feed sensor | Digital input | Debounced item-present event; no repeated triggers while occupied |
| Camera | Frame + metadata | Bounded capture; report failure explicitly |
| Classifier | valid, label, score, model version | Return invalid/UNKNOWN on model or preprocessing failure |
| Decision gate | Classifier result | Reject invalid, non-finite, or below-threshold scores |
| Release gate | Actuator command | Hold until route is selected and safe to release |
| Selector | HOME / route command | Bounded motion; calibrated positions; physical stops |
| Exit sensor | Digital input | Confirm passage before counting success |
| Event logger | Local event | Record class, confidence, route, duration, REVIEW/JAM/FAULT |

The firmware state machine owns the sequence. Individual modules must not independently start a second sorting cycle.

## 6. Provisional materials and components

This is a planning checklist, not a purchase order. Exact models and quantities depend on measured geometry and the chosen board.

| Item | Purpose | Selection status |
|---|---|---|
| ESP32-S3 board with PSRAM | Offline inference/controller | Candidate only; verify exact board |
| Compatible camera | Image capture | Not selected; verify pin map and driver support |
| Servo for selector | Four-way routing | Not selected; measure load and travel first |
| Optional second actuator | Release gate | Decide after passive mock-up |
| Object-present sensor | Detect incoming item | Not selected; verify output logic |
| Exit sensor | Confirm item passage | Not selected; verify sensing range |
| Diffused white LEDs | Stable imaging | Select after camera exposure tests |
| Regulated servo supply | Servo power | Size from selected servo stall current |
| Frame and bin material | Structure and collection | Select after mock-up |
| Connectors, wire, switch, fuse | Safe serviceable wiring | Finalize after current and pin map are known |

Do not copy example camera/servo names or pin numbers from concept art or generic tutorials into the build. Select components from official datasheets and verify compatibility as a set.

## 7. Mechanical and functional acceptance checklist

- [ ] Only one item enters the inspection chamber per cycle.
- [ ] All representative items pass without snagging in repeated feed trials.
- [ ] Camera framing and lighting remain fixed between captures.
- [ ] The release gate holds the item during capture and inference.
- [ ] Selector reaches every calibrated position without hitting its frame.
- [ ] UNKNOWN, invalid model output, and low confidence do not trigger normal sorting.
- [ ] Exit sensor confirms passage before a successful-sort count is recorded.
- [ ] Missing exit feedback causes a bounded JAM/REVIEW state.
- [ ] Bins can be removed without disturbing the camera or electronics.
- [ ] Servo power and logic power are wired according to the selected component specifications.
- [ ] The complete cycle works with internet access disabled.

## 8. Decisions still required before CAD and purchasing

1. Maximum accepted item dimensions and weight.
2. Whether REVIEW gets a fifth bin or a manual-access holding position.
3. Exact ESP32-S3 board and camera combination.
4. Exact servo(s), after measuring selector and gate loads.
5. Sensor type, mounting position, output voltage, and detection range.
6. Available fabrication methods and material thickness.
7. Final competition category definitions.

Record these decisions in `docs/HARDWARE.md` after they are verified. Until then, keep dimensions, GPIOs, servo angles, and component models explicitly provisional.
