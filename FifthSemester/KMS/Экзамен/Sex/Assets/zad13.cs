using UnityEngine;

public class zad13 : MonoBehaviour { //11,16pract 
    float speed = 0.1f;
    new AudioSource audio;

    void Start()
    {
        audio = GetComponent<AudioSource>();
    }
    void Update()
    {
        float x = Input.GetAxis("Horizontal") * speed;
        float z = Input.GetAxis("Vertical") * speed;
        transform.Translate(x, 0, z);
    }
    void OnCollisionEnter(Collision col)
    {
        if (!audio.isPlaying)
        {
            audio.Play();
        }
    }
}
