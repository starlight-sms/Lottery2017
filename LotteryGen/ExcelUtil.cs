using OfficeOpenXml;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LotteryGen
{
    public static class ExcelUtil
    {
        public static Dictionary<string, string> ReadExcelToNameQuotes(string path)
        {
            var nameQuotes = new Dictionary<string, string>();

            using (var file = File.OpenRead(path))
            using (var excel = new ExcelPackage(file))
            {
                var sheet = excel.Workbook.Worksheets["Sheet1"];
                var cells = sheet.Cells;
                for (var row = 1; ; ++row)
                {
                    var name = cells[row, 1].GetValue<string>();
                    var quotes = cells[row, 2].GetValue<string>();
                    if (name == null) break;

                    nameQuotes.Add(name, quotes);
                }
            }

            return nameQuotes;
        }
    }
}
