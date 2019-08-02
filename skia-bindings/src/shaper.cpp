// We always compile SkShaper.h with HARFBUZZ & ICU for now.
#define SK_SHAPER_HARFBUZZ_AVAILABLE

#include "SkShaper.h"
#include "SkFontMgr.h"

extern "C" SkShaper* C_SkShaper_MakePrimitive() {
    return SkShaper::MakePrimitive().release();
}

extern "C" SkShaper* C_SkShaper_MakeShaperDrivenWrapper() {
    return SkShaper::MakeShaperDrivenWrapper().release();
}

extern "C" SkShaper* C_SkShaper_MakeShapeThenWrap() {
    return SkShaper::MakeShapeThenWrap().release();
}

extern "C" SkShaper* C_SkShaper_Make() {
    return SkShaper::Make().release();
}

extern "C" void C_SkShaper_delete(SkShaper* self) {
    delete self;
}

extern "C" void C_SkShaper_RunIterator_delete(SkShaper::RunIterator* self) {
    delete self;
}

extern "C" void C_SkShaper_RunIterator_consume(SkShaper::RunIterator* self)  {
    self->consume();
}

extern "C" size_t C_SkShaper_RunIterator_endOfCurrentRun(const SkShaper::RunIterator* self) {
    return self->endOfCurrentRun();
}

extern "C" bool C_SkShaper_RunIterator_atEnd(const SkShaper::RunIterator* self) {
    return self->atEnd();
}

extern "C" const SkFont* C_SkShaper_FontRunIterator_currentFont(const SkShaper::FontRunIterator* self) {
    return &self->currentFont();
}

extern "C" SkShaper::FontRunIterator* C_SkShaper_MakeFontMgrRunIterator(const char* utf8, size_t utf8Bytes, const SkFont* font, SkFontMgr* fallback) {
    return SkShaper::MakeFontMgrRunIterator(utf8, utf8Bytes, *font, sk_sp<SkFontMgr>(fallback)).release();
}

extern "C" uint8_t C_SkShaper_BiDiRunIterator_currentLevel(const SkShaper::BiDiRunIterator* self) {
    return self->currentLevel();
}

extern "C" SkShaper::BiDiRunIterator* C_SkShaper_MakeIcuBidiRunIterator(const char* utf8, size_t utf8Bytes, uint8_t bidiLevel) {
    return SkShaper::MakeIcuBiDiRunIterator(utf8, utf8Bytes, bidiLevel).release();
}

extern "C" SkFourByteTag C_SkShaper_ScriptRunIterator_currentScript(const SkShaper::ScriptRunIterator* self) {
    return self->currentScript();
}

extern "C" SkShaper::ScriptRunIterator* C_SkShaper_MakeHbIcuScriptRunIterator(const char* utf8, size_t utf8Bytes) {
    return SkShaper::MakeHbIcuScriptRunIterator(utf8, utf8Bytes).release();
}

extern "C" const char* C_SkShaper_LanguageRunIterator_currentLanguage(const SkShaper::LanguageRunIterator* self) {
    return self->currentLanguage();
}

extern "C" SkShaper::LanguageRunIterator* C_SkShaper_MakeStdLanguageRunIterator(const char* utf8, size_t utf8Bytes) {
    return SkShaper::MakeStdLanguageRunIterator(utf8, utf8Bytes).release();
}

extern "C" void C_SkShaper_RunHandler_delete(SkShaper::RunHandler* self) {
    delete self;
}

extern "C" struct TraitObject {
    void* data;
    void* vtable;
};

namespace RunHandler {
    extern "C" typedef void (*BeginLine)(TraitObject);
    extern "C" typedef void (*RunInfo)(TraitObject, const SkShaper::RunHandler::RunInfo*);
    extern "C" typedef void (*CommitRunInfo)(TraitObject);
    extern "C" typedef SkShaper::RunHandler::Buffer (*RunBuffer)(TraitObject, const SkShaper::RunHandler::RunInfo*);
    extern "C" typedef void (*CommitRunBuffer)(TraitObject, const SkShaper::RunHandler::RunInfo*);
    extern "C" typedef void (*CommitLine)(TraitObject);
}

class RustRunHandler: SkShaper::RunHandler {

public:
    struct Param {
        TraitObject trait;
        ::RunHandler::BeginLine beginLine;
        ::RunHandler::RunInfo runInfo;
        ::RunHandler::CommitRunInfo commitRunInfo;
        ::RunHandler::RunBuffer runBuffer;
        ::RunHandler::CommitRunBuffer commitRunBuffer;
        ::RunHandler::CommitLine commitLine;
    };

    explicit RustRunHandler(const Param& param)
    :_param(param){
    }


private:
    void beginLine() override {
        _param.beginLine(_param.trait);
    }

    void runInfo(const RunInfo &info) override {
        _param.runInfo(_param.trait, &info);
    }

    void commitRunInfo() override {
        _param.commitRunInfo(_param.trait);
    }

    Buffer runBuffer(const RunInfo &info) override {
        return _param.runBuffer(_param.trait, &info);
    }

    void commitRunBuffer(const RunInfo &info) override {
        _param.commitRunBuffer(_param.trait, &info);
    }

    void commitLine() override {
        _param.commitLine(_param.trait);
    }

private:
    Param _param;
};

extern "C" void C_RustRunHandler_construct(RustRunHandler* uninitialized, const RustRunHandler::Param* param) {
    new(uninitialized)RustRunHandler(*param);
}

extern "C" void
C_SkShaper_shape(const SkShaper *self, const char *utf8, size_t utf8Bytes, const SkFont *srcFont, bool leftToRight,
                 SkScalar width, SkShaper::RunHandler *runHandler) {
    self->shape(utf8, utf8Bytes, *srcFont, leftToRight, width, runHandler);
}

extern "C" void
C_SkShaper_shape2(const SkShaper *self, const char *utf8, size_t utf8Bytes, SkShaper::FontRunIterator *fontRunIterator,
                  SkShaper::BiDiRunIterator *bidiRunIterator,
                  SkShaper::ScriptRunIterator *scriptRunIterator,
                  SkShaper::LanguageRunIterator *languageRunIterator, SkScalar width,
                  SkShaper::RunHandler *runHandler) {
    self->shape(utf8, utf8Bytes, *fontRunIterator, *bidiRunIterator, *scriptRunIterator, *languageRunIterator, width,
                runHandler);
}

extern "C" SkTextBlob* C_SkTextBlobBuilderRunHandler_makeBlob(SkTextBlobBuilderRunHandler* self) {
    return self->makeBlob().release();
}
