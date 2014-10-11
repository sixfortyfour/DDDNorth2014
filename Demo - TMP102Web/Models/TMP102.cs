using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Demo___TMP102Web.Models
{
    public class TMP102
    {
        public TMP102()
        {
            Readings = new List<Temperature>();
        }
        
        public List<Temperature> Readings { get; set; }
    }
}