using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Helpers;
using System.Web.UI;

using Demo___TMP102Web.Models;

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

        public ActionResult Push(double? reading) 
        {
            if (reading.HasValue)
                tmp102.Readings.Add(new Temperature() { Id = ++index, Reading = reading.Value });

            return PartialView(tmp102);
        }

        public ActionResult GetChartImage()
        {
            if (tmp102.Readings.Count > 0)
            {
                var key = new Chart(width: 600, height: 400)
                                .AddSeries(chartType: "Line", name: "Readings")
                                .DataBindTable(dataSource: tmp102.Readings, xField: "Id")
                                .SetXAxis("Count")
                                .SetYAxis("Temperature", 15.0, 30.0);

                return File(key.ToWebImage().GetBytes(), "image/jpeg");
            }
            else
            {
                return File("~/Content/blank.jpg", "image/jpeg");
            }
        }
    }
}