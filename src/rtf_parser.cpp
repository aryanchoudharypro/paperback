/* rtf_parser.cpp - RTF parsing implementation.
 * 
 * Paperback.
 * Copyright (c) 2025 Quin Gillespie.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 */

#include "rtf_parser.hpp"
#include "html_parser.hpp"
#include "app.hpp"
#include <wx/filename.h>
#include <wx/stdpaths.h>

std::span<const wxString> rtf_parser::extensions() const {
    static const wxString exts[] = {"rtf"};
    if (wxGetApp().is_soffice_found()) {
        return exts;
    }
    return {};
}

std::unique_ptr<document> rtf_parser::load(const wxString& path) const {
    wxFileName temp_dir(wxStandardPaths::Get().GetTempDir(), "paperback");
    if (!temp_dir.DirExists()) {
        temp_dir.Mkdir();
    }

    wxString command = wxString::Format("soffice --headless --convert-to html \"%s\" --outdir \"%s\"", path, temp_dir.GetPath());
    if (wxExecute(command, wxEXEC_SYNC) != 0) {
        return nullptr;
    }

    wxFileName html_path(temp_dir.GetPath(), wxFileName(path).GetName(), "html");

    html_parser parser;
    auto doc = parser.load(html_path.GetFullPath());

    wxRemoveFile(html_path.GetFullPath());

    return doc;
}

REGISTER_PARSER(rtf_parser);