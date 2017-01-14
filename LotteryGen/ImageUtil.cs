using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LotteryGen
{
    public static class ImageUtil
    {
        public static Dictionary<string, string> GetImages(string sourceDirectory)
        {
            return Directory.EnumerateFiles(sourceDirectory)
                .ToDictionary(k => Path.GetFileNameWithoutExtension(k), v => v);
        }
    }
}
