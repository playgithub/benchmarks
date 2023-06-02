#include <chrono>
#include <iostream>

#include <QtAssert>

#include <podofo/podofo.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>


using namespace PoDoFo;

void ExtractText_Podofo(const std::string_view & pdf_path, int page_index);
void ExtractText_Poppler(const std::string & pdf_path, int page_index);

int main(int argc, char *argv[])
{
    try
    {
        // Call the drawing routing which will create a PDF file
        // with the filename of the output file as argument.
        std::string pdf_path(u8"/path/to/pdf");

        typedef std::chrono::duration<long, std::ratio<1, 1000>> duration_ms;

        duration_ms duration_ms_for_ExtractText_Podofo;
        duration_ms duration_ms_for_ExtractText_Poppler;

        {
            auto start = std::chrono::high_resolution_clock::now();
            ExtractText_Podofo(pdf_path, 10);
            auto stop = std::chrono::high_resolution_clock::now();

            duration_ms_for_ExtractText_Podofo = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        }

        {
            auto start = std::chrono::high_resolution_clock::now();
            ExtractText_Poppler(pdf_path, 10);
            auto stop = std::chrono::high_resolution_clock::now();

            duration_ms_for_ExtractText_Poppler = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        }
        std::cout << "---------------------------------------------------------------------------\n";
        std::cout << "ExtractText_Podofo (ms): " << duration_ms_for_ExtractText_Podofo.count() << '\n';
        std::cout << "ExtractText_Poppler (ms): " << duration_ms_for_ExtractText_Poppler.count() << '\n';
        std::cout << "ExtractText_Podofo / ExtractText_Poppler = " << duration_ms_for_ExtractText_Podofo / duration_ms_for_ExtractText_Poppler << '\n';
        std::cout << "---------------------------------------------------------------------------" << std::endl;
    }
    catch (PdfError& err)
    {
        // We have to check if an error has occurred.
        // If yes, we return and print an error message
        // to the commandline.
        err.PrintErrorMsg();
        return (int)err.GetCode();
    }

    return 0;
}

void ExtractText_Podofo(const std::string_view & pdf_path, int page_index)
{
    PdfMemDocument doc;

    doc.Load(pdf_path);
    const PdfPageCollection & pages = doc.GetPages();
    unsigned int page_count = pages.GetCount();

    const PdfPage & page = pages.GetPageAt(page_index);

    std::vector<PdfTextEntry> entries;
    page.ExtractTextTo(entries);

    size_t entry_count = entries.size();

    for (size_t i = 0; i < entry_count; ++i)
    {
        std::cout << entries[i].Text << std::endl;
    }
}

void ExtractText_Poppler(const std::string & pdf_path, int page_index)
{
    poppler::document * doc = poppler::document::load_from_file(pdf_path);
    Q_ASSERT(doc != nullptr);

    int pages = doc->pages();

    poppler::page * page = doc->create_page(page_index);
    Q_ASSERT(page != nullptr);
    std::vector<poppler::text_box> text_boxes = page->text_list();
    size_t text_box_count = text_boxes.size();
    std::string text_utf8;
    for (size_t i = 0; i < text_box_count; ++i)
    {
        poppler::byte_array ba = text_boxes[i].text().to_utf8();
        text_utf8.assign(ba.data(), ba.size());
        std::cout << text_utf8 << std::endl;
    }

    delete page;

    delete doc;
}
