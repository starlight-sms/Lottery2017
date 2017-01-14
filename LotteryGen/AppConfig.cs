using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LotteryGen
{
    public static class AppConfig
    {
        public static string Source => ConfigurationManager.AppSettings[nameof(Source)];

        public static string Dest => ConfigurationManager.AppSettings[nameof(Dest)];

        public static string ExcelPath => ConfigurationManager.AppSettings[nameof(ExcelPath)];
    }
}
