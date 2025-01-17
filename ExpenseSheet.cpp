#include "ExpenseSheet.hpp"

void ExpenseSheet::Entry::Serialize(std::ostream &out) const
{
    out.write(label.c_str(), label.length() + 1);
    out.write((char *)&value, sizeof(double));
}

void ExpenseSheet::Entry::Deserialize(std::istream &in)
{
    std::stringstream ss;
    char c;
    do
    {
        in.read(&c, 1);
        if (c != '\0')
            ss << c;
    } while (c != '\0');

    label = ss.str();
    in.read((char *)&value, sizeof(double));
}

bool ExpenseSheet::Open(const std::__fs::filesystem::path &dataFile)
{
    auto path = dataFile;
    path.replace_extension(".mec");
    std::ifstream fileIn(path, std::ios::in | std::ios::binary);
    if (fileIn.is_open())
    {
        m_path = dataFile;

        size_t elementCount = 0;

        fileIn.read((char *)&elementCount, sizeof(size_t));
        entrys.clear();

        for (size_t i = 0; i < elementCount; i++)
        {
            Entry e;
            e.Deserialize(fileIn);
            entrys.push_back(std::move(e));
        }

        return true;
    }
    return false;
}

bool ExpenseSheet::Save(const std::__fs::filesystem::path &dataFile) const
{
    auto xpath = dataFile;

    if (dataFile.empty())
    {
        if (m_path.empty())
        {
            return false;
        }
        else
        {
            xpath = m_path;
        }
    }

    auto path = xpath;
    path.remove_filename();
    if (!path.empty())
        std::__fs::filesystem::create_directories(path);

    xpath.replace_extension(".mec");

    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc | std::ios::binary);
    if (fileOut.is_open())
    {
        size_t elementCount = entrys.size();
        fileOut.write((char *)&elementCount, sizeof(size_t));
        for (const Entry &e : entrys)
        {
            e.Serialize(fileOut);
        }
        return true;
    }
    return false;
}

void ExpenseSheet::New()
{
    m_path = "";
    entrys.clear();
}

bool ExpenseSheet::Add(std::string_view label, double value)
{
    Entry e;
    e.label = label;
    e.value = value;

    bool canInsert = std::find(entrys.begin(), entrys.end(), e) == entrys.end();
    if (canInsert)
    {
        entrys.push_back(std::move(e));
    }
    return canInsert;
}

bool ExpenseSheet::Del(std::string_view label)
{
    Entry e;
    e.label = label;

    auto element = std::find(entrys.begin(), entrys.end(), e);
    if (element != entrys.end())
    {
        entrys.erase(element);
        return true;
    }

    return false;
}
void ExpenseSheet::List(std::ostream &os) const
{
    size_t idx = 0;
    for (const Entry &e : entrys)
    {
        os << "(" << idx++ << ") " << e.label << " " << e.value << std::endl;
    }
}
double ExpenseSheet::Eval() const
{
    double value;
    for (const Entry &e : entrys)
    {
        value += e.value;
    }
    return value;
}

bool ExpenseSheet::ExportCsv(const std::__fs::filesystem::path &file) const
{
    auto xpath = file;
    xpath.replace_extension(".csv");

    auto path = xpath;
    if (!path.remove_filename().empty())
    {
        std::__fs::filesystem::create_directories(path);
    }
    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc);
    if (fileOut.is_open())
    {
        fileOut << "Pos,Expense,Value" << "\n";
        size_t i = 0;
        for (const Entry &e : entrys)
        {
            fileOut << ++i << "," << e.label << "," << e.value << "\n";
        }
        return true;
    }

    return false;
}

bool ExpenseSheet::ExportHtml(const std::__fs::filesystem::path &file) const
{
    auto xpath = file;
    xpath.replace_extension(".html");

    auto path = xpath;
    if (!path.remove_filename().empty())
    {
        std::__fs::filesystem::create_directories(path);
    }
    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc);
    if (fileOut.is_open())
    {
        fileOut <<
            R"(<!doctype html>
            <html lang="en" data-bs-theme="dark">
            <head>
                <meta charset="utf-8">
                <meta name="viewport" content="width=device-width, initial-scale=1">
                <title>Expenses)"
                << m_path.filename().string() << R"( </title>
                <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH" crossorigin="anonymous">
            </head>
            <body>
              <nav class="navbar bg-body-tertiary">
                <div class="container-fluid">
                  <span class="navbar-brand mb-0 h1">Expenses: )"
                << m_path.filename().string() << R"(<pan>
                </div>
              </nav>
              <div class="container mt-2">
                <div class="card">
                  <div class="card-body">
                    <table class="table table-hover">
                      <thead>
                        <tr>
                          <th scope="col">#</th>
                          <th scope="col">Expense</th>
                          <th scope="col">Value</th>
                        </tr>
                      </thead>
                      <tbody>
                        )";

        size_t idx = 0;
        for (const Entry& e : entrys)
        {
            fileOut << "<tr><th scope=\"row\">" << ++idx <<  "</th>" 
                << "<td>" << e.label << "</td>" 
                << "<td class=\""<< TextHTMLClass(e.value) << "\">" << e.value << "</td></tr>";
        }

        fileOut <<
            R"(      
                          </tbody>
                        </table>
                        <p class="fs-3 )" << TextHTMLClass(Eval()) << R"(">TOTAL: )" << Eval() << R"(</p>
                      </div>
                    </div>
                </div> 
                <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz" crossorigin="anonymous"></script>
              </body>
            </html>)";
        
        return true;
    }

    return false;
}
