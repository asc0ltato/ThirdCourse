using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ButtonTaskHandler1 : MonoBehaviour
{
    public Text infoText;
    public string elementDescription;

    public void ClickButton()
    {
        if (infoText != null)
        {
            infoText.text = elementDescription;
        }
    }
}