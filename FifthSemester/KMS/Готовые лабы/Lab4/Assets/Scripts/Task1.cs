using UnityEngine;
using UnityEngine.UI;

public class Task1 : MonoBehaviour
{
    public Animator anim1; 
    public Animator anim2; 
    public Button nextButton;
    public Button nextButton2;

    private bool isAnim1Finished = false;
    private bool isAnim2Finished = false;

    private string anim1StateName = "PlayForward"; 
    private string anim2StateName = "PereklForward";

    void Start()
    {
        nextButton.interactable = false; 
        nextButton2.interactable = false;
    }

    void Update()
    {
        AnimatorStateInfo anim1State = anim1.GetCurrentAnimatorStateInfo(0);
        AnimatorStateInfo anim2State = anim2.GetCurrentAnimatorStateInfo(0);

        if (anim1State.IsName(anim1StateName) && anim1State.normalizedTime >= 1f && !isAnim1Finished)
        {
            isAnim1Finished = true;
        }

        if (anim2State.IsName(anim2StateName) && anim2State.normalizedTime >= 1f && !isAnim2Finished)
        {
            isAnim2Finished = true;
        }

        if (isAnim1Finished && isAnim2Finished)
        {
            nextButton.interactable = true;
        }
    }
}
