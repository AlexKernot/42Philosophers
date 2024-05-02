/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 19:07:24 by akernot           #+#    #+#             */
/*   Updated: 2024/05/02 16:11:26 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include "philo.h"

void	wait_out(t_program_data *data, pthread_t *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(philos[i], NULL);
		++i;
	}
	return ;
}

static void	*mutex_error(int error_code)
{
	printf("[Error - fatal] ");
	if (error_code == EAGAIN)
		printf("Insufficient computer resources to initialize mutex.\n");
	if (error_code == ENOMEM)
		printf("The computer has run out of memory.\n");
	if (error_code == EPERM)
		printf("Insufficient permissions to create the mutex lock\n");
	if (error_code == EBUSY)
		printf("Attempted to initialize mutex at an invalid address.\n");
	if (error_code == EBUSY)
		printf("Attempting to initialize an already initialized mutex.\n");
	if (error_code == EDEADLK)
		printf("Mutex deadlock detected.\n");
	if (error_code == EPERM)
		printf("Unlocking mutex thread doesn't own.\n");
	return (NULL);
}

void	return_forks(t_philo *philo)
{
	if (philo->left_fork == philo->right_fork)
		return ;
	pthread_mutex_lock(&(philo->left_fork->lock));
	philo->left_fork->taken = false;
	pthread_mutex_unlock(&philo->left_fork->lock);
	pthread_mutex_lock(&(philo->right_fork->lock));
	philo->right_fork->taken = false;
	pthread_mutex_unlock(&(philo->right_fork->lock));
}

t_bool	take_forks(t_philo *philo)
{
	if (philo->left_fork == philo->right_fork)
		return (false);
	pthread_mutex_lock(&(philo->left_fork->lock));
	if (philo->left_fork->taken == true)
	{
		pthread_mutex_unlock(&philo->left_fork->lock);
		return (false);
	}
	philo->left_fork->taken = true;
	pthread_mutex_lock(&(philo->right_fork->lock));
	if (philo->right_fork->taken == true)
	{
		philo->left_fork->taken = false;
		pthread_mutex_unlock(&philo->left_fork->lock);
		pthread_mutex_unlock(&philo->right_fork->lock);
		return (false);
	}
	philo->right_fork->taken = true;
	pthread_mutex_unlock(&(philo->left_fork->lock));
	pthread_mutex_unlock(&(philo->right_fork->lock));
	return (true);
}

t_fork	*create_forks(t_program_data *data)
{
	t_fork	*forks;
	int		i;
	int		mutex_return;

	i = 0;
	forks = malloc(sizeof(t_fork) * data->num_philos);
	if (forks == NULL)
		return (NULL);
	while (i < data->num_philos)
	{
		mutex_return = pthread_mutex_init(&(forks[i].lock), NULL);
		if (mutex_return != 0)
			return (mutex_error(mutex_return));
		forks[i].taken = false;
		++i;
	}
	return (forks);
}
