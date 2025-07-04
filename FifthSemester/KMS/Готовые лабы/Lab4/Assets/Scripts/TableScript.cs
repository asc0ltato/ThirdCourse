using System;
using UnityEngine;
using UnityEngine.UI;

public class TableScript : MonoBehaviour
{
    [SerializeField] InputField textInput;
    [SerializeField] Text t11, t21, i11, u11, l11, k11; 
    [SerializeField] Text t12, t22, i12, u12, l12, k12; 
    [SerializeField] Text t13, t23, i13, u13, l13, k13; 
    [SerializeField] Text h1, h2, h3, hc, w, wm, vo, q1, q2, q3, Qcp, c; 

    private const float B = 101.325f; 
    private const float pi = 3.14159f; 
    private const float d = 0.15f; 
    private const float p = 0.898f;
    private const float psi = 0.81f;
    private const float g = 9.8f;

    public void WriteValue()
    {
        if (t11.text == "-")
        {
            t11.text = t12.text = t13.text = textInput.text.ToString();
        }
        else if (t21.text == "-")
        {
            t21.text = t22.text = t23.text = textInput.text.ToString();
        }
        else if (i11.text == "-")
        {
            i11.text = i12.text = i13.text = textInput.text.ToString();
        }
        else if (u11.text == "-")
        {
            u11.text = u12.text = u13.text = textInput.text.ToString();
            q1.text = (float.Parse(u11.text) * float.Parse(i11.text)).ToString();
            q2.text = (float.Parse(u12.text) * float.Parse(i12.text)).ToString();
            q3.text = (float.Parse(u13.text) * float.Parse(i13.text)).ToString();
            Qcp.text = ((float.Parse(q1.text) + float.Parse(q2.text) + float.Parse(q3.text)) / 3).ToString();
        }
        else if (l11.text == "-")
        {
            l11.text = l12.text = l13.text = textInput.text.ToString();
        }
        else if (k11.text == "-")
        {
            k11.text = k12.text = k13.text = textInput.text.ToString();
            h1.text = (g * float.Parse(k11.text) * float.Parse(l11.text)).ToString("F1");
            h2.text = (g * float.Parse(k12.text) * float.Parse(l12.text)).ToString("F1");
            h3.text = (g * float.Parse(k13.text) * float.Parse(l13.text)).ToString("F1");
            hc.text = ((float.Parse(h1.text) + float.Parse(h2.text) + float.Parse(h3.text)) / 3).ToString("F1");
            w.text = (Mathf.Sqrt((2 * float.Parse(hc.text)) / p)).ToString("F1");
            wm.text = ((float.Parse(w.text) * psi)).ToString("F1");
            vo.text = ((pi * Mathf.Pow(d, 2) / 4) * float.Parse(wm.text) * ((B / 101.325f) * (273.15f / (273.15f + float.Parse(t12.text))))).ToString("F1");
            c.text = (float.Parse(Qcp.text) / (float.Parse(vo.text) * (float.Parse(t21.text) - float.Parse(t11.text)))).ToString("F1");
        }

        textInput.text = "";
    }

    public void Clean()
    {
        t11.text = "-";
        t21.text = "-";
        t12.text = "-";
        t22.text = "-";
        t13.text = "-";
        t23.text = "-";
        u11.text = "-";
        u12.text = "-";
        u13.text = "-";
        i11.text = "-";
        i12.text = "-";
        i13.text = "-";
        l11.text = "-";
        l12.text = "-";
        l13.text = "-";
        k11.text = "-";
        k12.text = "-";
        k13.text = "-";
        h1.text = "-";
        h2.text = "-";
        h3.text = "-";
        hc.text = "-";
        w.text = "-";
        wm.text = "-";
        vo.text = "-";
        q1.text = "-";
        q2.text = "-";
        q3.text = "-";
        Qcp.text = "-";
        c.text = "-";
    }
}