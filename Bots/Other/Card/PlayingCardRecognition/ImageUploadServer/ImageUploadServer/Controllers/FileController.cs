using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web;
using System.Linq;
using System.Web.Http;

namespace ImageUploadServer.Controllers
{
    public class FileController : ApiController
    {

        CardRecognizer recognizer = new CardRecognizer();

        [HttpPost]
        public IHttpActionResult UploadImage()
        {
            var fileUploaded = false;

            CardCollection returnData=null;

            
            foreach (string upload in HttpContext.Current.Request.Files)
            {

                if (!HasFile(HttpContext.Current.Request.Files[upload]))
                    continue;

                string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "uploads");

                string filename = Path.GetFileName(HttpContext.Current.Request.Files[upload].FileName);

                
                if (!Directory.Exists(path))
                    Directory.CreateDirectory(path);

                HttpContext.Current.Request.Files[upload].SaveAs(Path.Combine(path, filename));
                fileUploaded = true;

                Image savedImage=Image.FromFile(path+@"\"+filename);

                Bitmap bitmap = new Bitmap(savedImage);
                

                returnData =recognizer.Recognize(bitmap);

                savedImage = null;
            }

            if (fileUploaded)
            {
                if (returnData != null && returnData.Count > 0)
                    return Json(returnData[0].ToString());
                else
                    return Json("No cards");
            }
            else
                return Json("Error");
        }

        private static bool HasFile(HttpPostedFile file)
        {
            return (file != null && file.ContentLength > 0) ? true : false;
        }
    }
}
