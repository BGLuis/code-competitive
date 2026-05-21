.PHONY: all pdf clean

# Output PDF file name
OUTPUT = CP_Reference.pdf

# Input files (sorted alphabetically by directory)
INPUTS = docs/data-structure/*.md docs/algorithms/*.md docs/math/*.md

all: pdf

pdf:
	@echo "Generating PDF using Pandoc inside Docker..."
	@echo "This might take a moment to download the pandoc/extra image on the first run."
	docker run --rm \
		-v "$$(pwd):/data" \
		pandoc/extra \
		--pdf-engine=xelatex \
		-V geometry:margin=0.5in \
		-V monofont="Inconsolata" \
		--toc \
		--toc-depth=2 \
		$(INPUTS) -o $(OUTPUT)
	@echo "PDF generation complete: $(OUTPUT)"

clean:
	@echo "Cleaning up..."
	rm -f $(OUTPUT)
