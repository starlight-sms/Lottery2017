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
        public static Dictionary<string, Person> ReadExcelToNameQuotes(string path)
        {
            var nameQuotes = new Dictionary<string, Person>();

            using (var file = File.OpenRead(path))
            using (var excel = new ExcelPackage(file))
            {
                var sheet = excel.Workbook.Worksheets["Sheet1"];
                var cells = sheet.Cells;
                for (var row = 1; ; ++row)
                {
                    var p = new Person
                    {
                        Id = row, 
                        Name = cells[row, 1].GetValue<string>(), 
                        Quote = cells[row, 2].GetValue<string>(), 
                        IsNew = !string.IsNullOrWhiteSpace(cells[row, 3].GetValue<string>())
                    };
                    if (p.Name == null) break;

                    nameQuotes.Add(p.Name, p);
                }
            }

            return nameQuotes;
        }
    }

    public class Person
    {
        public int Id { get; set; }

        public string Name { get; set; }

        public string Quote { get; set; }

        public bool IsNew { get; set; }
    }
}
