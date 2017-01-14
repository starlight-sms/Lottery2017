using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LotteryGen
{
    public static class ProcessUtil
    {
        public static void WritePersonConfig(string destination,
            Dictionary<string, string> excels)
        {
            var filename = Path.Combine(destination, "Person.txt");
            const string Template = "{{ L\"{0}\", IDR_PERSON{1}, LR\"~({2})~\" }},";
            using (var file = File.OpenWrite(filename))
            using (var writer = new StreamWriter(file))
            {
                var id = 1;
                foreach (var kv in excels)
                {
                    writer.WriteLine(string.Format(Template, kv.Key, id++, kv.Value));
                }
            }
        }

        public static void CheckConflicts(
            Dictionary<string, string> excels, 
            Dictionary<string, string> images)
        {
            var excelsExcept = excels.Keys.Except(images.Keys).ToList();
            if (excelsExcept.Count > 0)
            {
                Console.WriteLine("Names existed in excel, but not in images: ");
                Console.WriteLine(string.Join("\t", excelsExcept));
            }

            var imagesExcept = images.Keys.Except(excels.Keys).ToList();
            if (imagesExcept.Count > 0)
            {
                Console.WriteLine("Names existed in images, but not in excel: ");
                Console.WriteLine(string.Join("\t", imagesExcept));
            }
        }
    }
}
