# Waste image dataset

Do not commit a large image collection until source licensing and repository size are reviewed. Split by physical item/session where possible so near-identical frames do not leak into both training and test sets.

Proposed folder layout:

    datasets/waste/
      train/paper/
      train/plastic/
      train/metal/
      train/other/
      val/paper/
      val/plastic/
      val/metal/
      val/other/
      test/paper/
      test/plastic/
      test/metal/
      test/other/

## Capture guidance
- Photograph actual items in the intended tray and camera distance.
- Include multiple objects, brands, colors, shapes, lighting conditions and clean/dirty states.
- Avoid near-identical frames across train/validation/test.
- Include only labels the camera can meaningfully distinguish.
- Decide how ambiguous composites (for example, paper cups or multilayer packaging) should be labelled.
- Record source, permission/license and assigned label.

The four classes are provisional. Update them to match the official competition brief before training.
