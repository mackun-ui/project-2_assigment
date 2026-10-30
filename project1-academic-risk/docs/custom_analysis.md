# Custom Analysis: Academic Risk Index (ARI)

**Purpose:** Measures how far a student's grade falls below a safe level,
so that support can be prioritised. It goes beyond pass/fail: a student with 55
passes (threshold 50) but is already close to the danger zone.

**Formula:** ARI = max(0, (60 - grade) / 60) x 100
- SAFE_LEVEL = 60 (grade at or above this has ARI 0)
- Grade 0 gives ARI 100

**Levels:** 0 = Safe | 0-25 Low (grade 45-60) | 25-50 Moderate (grade 30-45)
| above 50 High (grade below 30)

**Algorithm:** Traverse all records with a pointer, compute each ARI, count
students per level, average the ARIs, and track the lowest-grade student.

**Interpretation:** A high average class ARI or any High students means
intervention is needed. Example: grade 38 gives ARI 36.7 (Moderate).