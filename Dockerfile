FROM pandoc/extra

# Install system fonts so XeLaTeX and fontspec can find them natively
RUN apk update && apk add --no-cache \
    ttf-inconsolata \
    ttf-dejavu \
    fontconfig \
    && fc-cache -fv
