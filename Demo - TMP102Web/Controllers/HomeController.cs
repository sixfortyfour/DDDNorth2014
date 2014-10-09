using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

using Demo___TMP102Web.Models;

namespace Demo___TMP102Web.Controllers
{
    public class HomeController : Controller
    {
        static TMP102 tmp102 = new TMP102();

        public ActionResult Index()
        {
            return View(tmp102);
        }

        public ActionResult Push(int? id) 
        {
            if (id.HasValue)
                tmp102.Values.Add(id.Value);

            return View(tmp102);
        }
    }
}