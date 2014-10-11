using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

using Demo___TMP102Web.Models;
using System.Web.Helpers;

namespace Demo___TMP102Web.Controllers
{
    public class HomeController : Controller
    {
        static TMP102 tmp102 = new TMP102();
        static int index = 0;

        public ActionResult Index()
        {
            return View(tmp102);
        }

        public ActionResult Push(int? id) 
        {
            if (id.HasValue)
                tmp102.Readings.Add(new Temperature() { Id = ++index, Reading = id.Value });

            return View(tmp102);
        }

        public ActionResult GetChartImage()
        {
            var key = new Chart(width: 300, height: 300)
            .AddTitle("Temperature Chart")
            .AddSeries(
            chartType: "Line",
            name: "Readings",
            xValue: tmp102.Readings.Select(x => x.Id).ToArray(),
            yValues: tmp102.Readings.Select(x => x.Reading).ToArray());

            return File(key.ToWebImage().GetBytes(), "image/jpeg");
        }
    }
}