# Waste image dataset guide

The proposed classes are paper, plastic, metal and other. Confirm event rules and target waste types before collecting data; the class list may need to change.

## Suggested capture protocol
- Photograph the item in the intended inspection area and camera viewpoint.
- Vary lighting, orientation, distance, backgrounds and item condition.
- Include clean, dirty, crushed, folded and partly occluded examples where appropriate.
- Use multiple distinct physical items per class; avoid mostly repeated shots of one object.
- Avoid faces, names, addresses, screens and other unnecessary personal information in frames.
- Label ambiguous, mixed-material or composite items according to a written rule. If uncertain, exclude it or reserve it for manual review rather than guessing.

## Split policy
Use datasets/waste/{train,val,test}/{paper,plastic,metal,other}/.
- train: fit model parameters.
- val: select models and thresholds.
- test: held back until model selection is complete.

Where possible, split by physical item/session rather than randomly splitting near-duplicate frames. Near-identical images across splits can make evaluation look better than real performance.

Run python3 tools/check_dataset.py datasets/waste to check that each folder exists and contains supported image extensions. The script does not decode images, verify labels, detect duplicates, or train a model.

## Evaluation before deployment
Report sample counts per class, per-class precision and recall, confusion matrix, and fraction rejected as UNKNOWN. Test separate real items from the intended environment. Do not choose a confidence threshold only to maximize overall accuracy; consider the cost of mis-sorting each class.
