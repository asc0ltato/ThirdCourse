using UnityEngine;

public class HelpButtonHandler : MonoBehaviour {

	public GameObject HelpPanel;

    public void ShowHelp()
    {
        HelpPanel.SetActive(true);
    }

    public void HideHelp()
    {
        HelpPanel.SetActive(false);
    }
}