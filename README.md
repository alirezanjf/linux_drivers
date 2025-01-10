# Uploading on Git

1. Check the Remote URL

Run the following command to see the remote repository URL:
bash
Copy

git remote -v

Example Output:
Copy

origin  https://github.com/username/repo-name.git (fetch)
origin  https://github.com/username/repo-name.git (push)

    origin is the default name for the remote repository.

    The URL after origin is the location where your files are being pushed.

2. Check the Current Branch and Remote

To see which branch you're pushing to and its upstream remote, run:
bash
Copy

git branch -vv

Example Output:
Copy

* main  abc1234 [origin/main] Commit message

    main is the current branch.

    origin/main indicates that the main branch is tracking the main branch on the origin remote.

3. Check the Push URL (if different from fetch)

If the push URL is different from the fetch URL, you can check it explicitly:
bash
Copy

git remote show origin

Example Output:
Copy

* remote origin
  Fetch URL: https://github.com/username/repo-name.git
  Push  URL: https://github.com/username/repo-name.git
  HEAD branch: main
  Remote branch:
    main tracked
  Local branch configured for 'git pull':
    main merges with remote main
  Local ref configured for 'git push':
    main pushes to main (up to date)

4. Change the Remote URL (if needed)

If you want to change where you're pushing files, update the remote URL:
bash
Copy

git remote set-url origin https://github.com/username/new-repo-name.git
