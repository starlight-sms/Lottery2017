using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LotteryGen
{
    class Program
    {
        static void Main(string[] args)
        {
            var excels = ExcelUtil.ReadExcelToNameQuotes(AppConfig.ExcelPath);
            var images = ImageUtil.GetImages(AppConfig.Source);            
            ProcessUtil.CheckConflicts(excels, images);
            ProcessUtil.WritePersonConfig(AppConfig.Dest, excels);
        }
    }
}
