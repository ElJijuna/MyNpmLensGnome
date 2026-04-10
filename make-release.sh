#!/usr/bin/env bash
set -e

APP_ID="io.github.eljijuna.NpmLens"
MANIFEST="${APP_ID}.json"
BUILD_DIR=".flatpak-build"
REPO_DIR=".flatpak-repo"
BUNDLE="${APP_ID}.flatpak"

echo "==> Building Flatpak..."
flatpak-builder --force-clean --repo="${REPO_DIR}" "${BUILD_DIR}" "${MANIFEST}"

echo "==> Creating bundle ${BUNDLE}..."
flatpak build-bundle "${REPO_DIR}" "${BUNDLE}" "${APP_ID}"

echo "==> Done: ${BUNDLE}"
