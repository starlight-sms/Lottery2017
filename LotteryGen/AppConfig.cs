using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Environment;

namespace LotteryGen
{
    public static class AppConfig
    {
        public static string Source => ConfigurationManager.AppSettings[nameof(Source)]
            .Replace("%Desktop%", GetFolderPath(SpecialFolder.Desktop));

        public static string Dest => ConfigurationManager.AppSettings[nameof(Dest)]
            .Replace("%Desktop%", GetFolderPath(SpecialFolder.Desktop));

        public static string ExcelPath => ConfigurationManager.AppSettings[nameof(ExcelPath)]
            .Replace("%Desktop%", GetFolderPath(SpecialFolder.Desktop));
    }
}
