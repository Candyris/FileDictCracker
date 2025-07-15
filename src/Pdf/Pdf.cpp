#include "Pdf.h"
#include <poppler/cpp/poppler-document.h>
#include <memory>
#include <cstdio>
#include <io.h>     // _dup, _dup2, _close
#include <fcntl.h>  // _fileno
#include <iostream>

static int saved_stderr_fd = -1;

#include <io.h>     // _dup, _dup2, _close
#include <fcntl.h>  // _fileno
#include <iostream>
#include <mutex>


bool Pdf::PasswordCheck(const char* path, const char* password)
{
    ScopedStderrSuppressor suppress; // Ensures stderr is restored no matter what

    poppler::document* raw_doc = {};
    raw_doc = poppler::document::load_from_file(path, password);
    
    std::unique_ptr<poppler::document> doc(raw_doc);

    return raw_doc && !raw_doc->is_locked();
}

ScopedStderrSuppressor::ScopedStderrSuppressor()
{
    fflush(stderr);
    saved_fd = _dup(_fileno(stderr));
    freopen("nul", "w", stderr);
}

ScopedStderrSuppressor::~ScopedStderrSuppressor()
{
    if (saved_fd != -1) {
        fflush(stderr);
        _dup2(saved_fd, _fileno(stderr));
        _close(saved_fd);
    }
}
