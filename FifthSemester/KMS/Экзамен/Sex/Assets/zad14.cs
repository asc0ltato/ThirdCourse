using UnityEngine;

public class zad14 : MonoBehaviour {
	public Animator anim;

	void Start () {
        anim = GetComponent<Animator>();
        anim.SetBool("run", false);
	}
	
	public void PlayAnim() {
		anim.SetBool("run", true);
    }
}