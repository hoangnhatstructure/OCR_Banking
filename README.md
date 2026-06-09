# LC OCR CPP v0.1

Prototype C++ skeleton for an offline LC / trade-document OCR extraction system.

## What this version does

- Reads case list from `config/Control.csv`
- Scans each case folder for `.txt`, `.pdf`, `.jpg`, `.png`, `.tif`, `.tiff`, `.bmp`
- Uses `ocr_text.txt` as a mock OCR source for the first test
- Keeps PDF/image input detection in the architecture
- Unwraps broken lines based on field aliases
- Extracts fields using `config/field_dictionary.csv` and `config/alias_dictionary.csv`
- Exports audit CSV with confidence and review flags
- Uses `std::filesystem`, so paths with spaces are supported

## What this version does not yet do

- It does not bundle Tesseract or Poppler.
- PDF/image OCR is currently a placeholder in `src/core/OCRRunner.cpp`.
- The first working test uses `workspace/input_documents/case 001/ocr_text.txt`.

This is intentional: v0.1 validates architecture, config, path handling, line unwrapping, extraction, and audit output before adding the real OCR engine.

## Folder structure

```text
LC_OCR_CPP_v0_1/
├── CMakeLists.txt
├── config/
│   ├── Control.csv
│   ├── field_dictionary.csv
│   ├── alias_dictionary.csv
│   └── app_config.json
├── src/
│   ├── main.cpp
│   ├── app/
│   ├── core/
│   └── io/
├── workspace/
│   ├── input_documents/
│   ├── ocr_output/
│   ├── output_audit/
│   ├── temp/
│   └── logs/
└── tools/
    ├── poppler/
    └── tesseract/
```

## Build

From project root:

```bash
cmake -S . -B build
cmake --build build
```

On Windows with Release config:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\LCOCR.exe
```

If using single-config generators such as MinGW Makefiles or Ninja:

```powershell
.\build\LCOCR.exe
```

## First test

`config/Control.csv` currently has:

```csv
case_id,input_folder,enabled,run_ocr,export_audit
case_001,"workspace/input_documents/case 001",1,0,1
```

`run_ocr=0` means the engine reads the existing mock OCR text file:

```text
workspace/input_documents/case 001/ocr_text.txt
```

Expected output:

```text
workspace/output_audit/case_001_audit.csv
workspace/ocr_output/case_001/combined_ocr_text.txt
workspace/logs/run.log
```

## Next implementation step

Add real offline OCR inside:

```cpp
src/core/OCRRunner.cpp
OCRRunner::runExternalOCRPlaceholder(...)
```

Recommended offline tools:

- Tesseract for OCR
- Poppler or MuPDF for PDF page rendering
- Optional OpenCV/Leptonica for preprocessing

Keep the rest of the pipeline unchanged.
